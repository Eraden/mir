#include <utility>

//
// Created by eraden on 10.02.19.
//

#include <mir/UpMigrator.hpp>
#include <mir/CommandLineInterface.hpp>
#include <mir/MigrationDirectoryReader.hpp>
#include <mir/LoadMigrationsQuery.hpp>
#include <mir/RunMigrationQuery.hpp>

namespace UpMigrator {
    UpMigrator::UpMigrator(CommandLineInterface *cli) :
            cli(cli),
            url(cli->url),
            connection(std::make_unique<Database::Connection>(cli->url->c_str(), cli->logger)) {
    }

    bool UpMigrator::run() {
        LoadMigrationsQuery query;
        if (!connection->execute(&query)) {
            return false;
        }
        auto rows = query.getResult();
        auto entries = MigrationDirectoryReader::readMigrationDirectory(cli);

        for (const auto &e : entries) {
            auto exists = std::any_of(rows.begin(), rows.end(), [&](const auto &row) {
                return row.migration == e.migration;
            });
            if (!exists) {
                const auto up = e.path / fs::path("up.psql");
                if (fs::exists(up)) {
                    fs::ifstream ifs(up);
                    std::stringstream ss;
                    ss << "BEGIN;\n";
                    ss << ifs.rdbuf();
                    ss << "\nINSERT INTO _mir_migrations(migration) VALUES (" << e.migration << ");\n";
                    ss << "COMMIT;\n";
                    auto contents = ss.str();
                    RunMigrationQuery runQuery(contents);
                    if (!connection->executeSql(&runQuery))
                        return false;
                }
            }
        }

        return true;
    }
}
