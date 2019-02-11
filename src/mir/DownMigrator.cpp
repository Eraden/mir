//
// Created by eraden on 10.02.19.
//

#include <mir/MigrationDirectoryReader.hpp>
#include <mir/DownMigrator.hpp>
#include <mir/CommandLineInterface.hpp>
#include <mir/LoadMigrationsQuery.hpp>
#include <mir/RunMigrationQuery.hpp>

namespace DownMigrator {
    DownMigrator::DownMigrator(CommandLineInterface *cli) :
            cli(cli),
            url(std::move(cli->url)),
            connection(std::make_unique<Database::Connection>(cli->url->c_str(), cli->logger)) {
    }

    bool DownMigrator::run() {
        LoadMigrationsQuery query;
        if (!connection->execute(&query)) {
            return false;
        }
        auto rows = query.getResult();
        if (rows.empty())
            return false;

        auto entries = MigrationDirectoryReader::readMigrationDirectory(cli);

        const auto &row = rows.back();
        for (const auto &e : entries) {
            if (e.migration == row.migration) {
                std::stringstream ss;
                ss << "BEGIN;\n";
                const auto down = e.path / "down.psql";
                if (fs::exists(down)) {
                    fs::ifstream ifs(down);
                    ss << ifs.rdbuf();
                }
                ss << ";\nDELETE FROM _mir_migrations WHERE migration = " << e.migration << ";\n";
                ss << "COMMIT;\n";
                auto contents = ss.str();
                RunMigrationQuery runQuery(contents);
                connection->executeSql(&runQuery);
                break;
            }
        }

        return true;
    }
}
