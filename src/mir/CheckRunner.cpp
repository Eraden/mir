//
// Created by eraden on 17.07.19.
//

#include <mir/mir/CheckRunner.hpp>

namespace CheckRunner {

    CheckRunner::CheckRunner(CommandLineInterface *cli) :
            cli(cli),
            url(cli->url),
            connection(std::make_unique<Database::Connection>(cli->url->c_str(), cli->logger)) {
    }

    bool CheckRunner::run() {
        LoadMigrationsQuery query;
        if (!connection->execute(&query)) {
            return false;
        }
        auto rows = query.getResult();

        auto entries = MigrationDirectoryReader::readMigrationDirectory(cli);
        std::sort(entries.begin(), entries.end(), [&](auto &a, auto &b) {
            return b.migration - a.migration;
        });

        std::vector<MigrationDirectoryReader::MigrationDirectory> missing;
        for (const auto &entry : entries) {
            auto found = false;
            for (const auto &row : rows) {
                if (entry.migration == row.migration) {
                    found = true;
                    break;
                }
            }
            if (!found) missing.push_back(entry);
        }
        if (!missing.empty()) {
            this->cli->logger->error("Missing files:");
            for (const auto &row : missing) {
                this->cli->logger->error("\t" + row.name);
            }
        }

        return missing.empty();
    }
}
