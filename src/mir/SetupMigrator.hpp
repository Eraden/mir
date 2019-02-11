//
// Created by eraden on 10.02.19.
//

#pragma once

#include <string>
#include <memory>
#include <db/Connection.hpp>
#include <db/Query.hpp>
#include <db/Url.hpp>

class CommandLineInterface;

namespace SetupMigrator {
    class SetupMigrator {
    public:
        explicit SetupMigrator(CommandLineInterface *cli);

        bool run();

    private:
        CommandLineInterface *cli = nullptr;
        std::shared_ptr<Database::Url> url;

        bool createDatabase();

        bool createMigrationsTable();
    };

    class CreateDatabaseQuery : public Database::Query<int> {
    public:
        explicit CreateDatabaseQuery(const std::string &dbName);

        const char *query() override;

        const char **argv() override;

        const int argc() override;


        void fetchResult(PGresult *result) override {}

        std::vector<int> getResult() override { std::vector<int> v; return v; }
    private:
        const std::string dbName;
        const std::string queryString;
    };

    class CreateMigrationTableQuery : public Database::Query<int> {
    public:
        CreateMigrationTableQuery();

        const char *query() override;

        const char **argv() override;

        const int argc() override;

        void fetchResult(PGresult *result) override {}

        std::vector<int> getResult() override { std::vector<int> v; return v; }
    };
}
