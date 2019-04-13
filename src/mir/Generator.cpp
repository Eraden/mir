//
// Created by eraden on 11.02.19.
//

#include <mir/mir/Generator.hpp>
#include <mir/mir/CommandLineInterface.hpp>

namespace mir {
    TableGenerator::TableGenerator(
            CommandLineInterface *cli,
            const std::string &tableName,
            const std::vector<std::string> &fields
    ) :
            tableName(tableName),
            cli(cli) {
        for (const auto &v : fields) {
            parseColumn(v);
        }
        if (cli->putTimestamps) {
            bool createdAtExists = false;
            bool updatedAtExists = false;
            for (const auto &f : this->fields) {
                if (f->name == "created_at") createdAtExists = true;
                if (f->name == "updated_at") updatedAtExists = true;
            }
            if (!createdAtExists) parseColumn("created_at:timestamp:NOW()");
            if (!updatedAtExists) parseColumn("updated_at:timestamp:NOW()");
        }
    }

    void TableGenerator::parseColumn(const std::string &columnSpec) {
        auto rest = columnSpec;
        std::string name;
        std::string type;
        auto pos = rest.find_first_of(':');
        if (pos == std::string::npos) {
            name = rest;
            type = cli->defaultType;
            rest = "";
        } else {
            name = rest.substr(0, pos);
            rest = rest.substr(pos + 1);
        }
        pos = rest.find_first_of(':');
        if (pos != std::string::npos) {
            type = rest.substr(0, pos);
            rest = rest.substr(pos + 1);
        } else if (!rest.empty()) {
            type = rest;
            rest = "";
        }
        if (type.empty()) type = cli->defaultType;
        auto field = std::make_shared<ColumnSpecification>(name, type);
        fields.push_back(field);

        if (rest.empty()) {
            return;
        }
        pos = rest.find_first_of(':');
        if (pos == std::string::npos) {
            field->defaultValue = rest;
            rest = "";
        } else {
            field->defaultValue = rest.substr(0, pos);
            rest = rest.substr(pos + 1);
        }
    }

    void TableGenerator::run() {
        auto migrationsDir = cli->getMigrationsDir();
        if (!fs::exists(migrationsDir)) {
            cli->logger->error("Migration directory does not exists!");
            std::exit(1);
        }
        std::stringstream ss;
        std::time_t t = std::time(nullptr);
        std::tm *now = std::localtime(&t);
        char time[1024];
        strftime(time, 1024, "%Y%m%d%H%M%S", now);
        ss << time << "_" << tableName;
        auto dir = migrationsDir / ss.str();
        fs::create_directory(dir);

        fs::ofstream upStream(dir / "up.psql");
        upStream << "CREATE TABLE " << tableName << " (" << std::endl;
        for (auto it = fields.begin(), end = fields.end(); it != end; ++it) {
            const auto &field = *it;
            upStream << "  " << field->name << " " << field->type;
            if (!field->defaultValue.empty()) {
                upStream << " DEFAULT " << field->defaultValue;
            }
            if (isPrimaryKey(field)) {
                upStream << " PRIMARY KEY UNIQUE";
            }
            if (field->type == "serial") {
                upStream << " NOT NULL";
            }
            if (it + 1 != end) upStream << ",";
            upStream << std::endl;
        }
        upStream << ");" << std::endl;

        fs::ofstream downStream(dir / "down.psql");
        downStream << "DROP TABLE " << tableName << ";" << std::endl;
    }

    bool TableGenerator::isPrimaryKey(const shared_ptr<ColumnSpecification> &field) const {
        return any_of(
                cli->primaryKeys.begin(),
                cli->primaryKeys.end(),
                [&](const auto &s) { return s == field->name; }
        );
    }

    bool Generator::match(const std::string &value) {
        return value == "gen" || value == "g" || value == "gt";
    }

    Generator::Generator(CommandLineInterface *cli, int argc, char **argv) :
            cli(cli) {
        for (auto i = 0; i < argc; i++) this->argv.emplace_back(argv[i]);
    }

    void Generator::run() {
        auto it = argv.begin();
        auto end = argv.end();
        for (; it != end; ++it) {
            const auto &v = *it;

            if ((v == "gen" || v == "g") && it + 3 != end && (*(it + 1) == "table" || *(it + 1) == "t")) {
                auto tableName = *(it + 2);
                it += 3;
                std::vector<std::string> columns;
                for (; it != end; ++it) columns.push_back(*it);
                TableGenerator generator(cli, tableName, columns);
                generator.run();
                break;
            } else if (v == "gt" && it + 2 != end) {
                auto tableName = *(it + 1);
                it += 2;
                std::vector<std::string> columns;
                for (; it != end; ++it) columns.push_back(*it);
                TableGenerator generator(cli, tableName, columns);
                generator.run();
                break;
            }
        }
    }
}
