//
// Created by eraden on 10.02.19.
//

#include "mir/SetupMigrator.hpp"
#include <mir/CommandLineInterface.hpp>

namespace SetupMigrator {
    SetupMigrator::SetupMigrator(CommandLineInterface *cli) :
            cli(cli),
            url(std::move(cli->url)) {
    }

    bool SetupMigrator::run() {
        createDatabase();
        return createMigrationsTable();
    }

    bool SetupMigrator::createDatabase() {
        auto connection = std::make_unique<Database::Connection>("postgres://localhost/postgres", cli->logger);
        CreateDatabaseQuery createDatabaseQuery(url->database);
        connection->execute(&createDatabaseQuery);
        return false;
    }

    bool SetupMigrator::createMigrationsTable() {
        auto connection = std::make_unique<Database::Connection>(url->c_str(), cli->logger);
        CreateMigrationTableQuery query;
        connection->execute(&query);
        return false;
    }

    const char *QUERY_STRING = "CREATE DATABASE ";

    CreateDatabaseQuery::CreateDatabaseQuery(const std::string &dbName) :
            Database::Query<int>(1),
            dbName(dbName),
            queryString(string(QUERY_STRING) + dbName) {
    }

    const char *CreateDatabaseQuery::query() {
        return queryString.c_str();
    }

    const char **CreateDatabaseQuery::argv() {
        return nullptr;
    }

    const int CreateDatabaseQuery::argc() {
        return 0;
    }

    static const char *CREATE_MIGRATION_TABLE = R"(
        CREATE TABLE _mir_migrations (
            id SERIAL PRIMARY KEY NOT NULL UNIQUE,
            migration BIGINT NOT NULL UNIQUE
        );
    )";

    const char *CreateMigrationTableQuery::query() {
        return CREATE_MIGRATION_TABLE;
    }

    const char **CreateMigrationTableQuery::argv() {
        return nullptr;
    }

    const int CreateMigrationTableQuery::argc() {
        return 0;
    }

    CreateMigrationTableQuery::CreateMigrationTableQuery() : Database::Query<int>(2) {
    }
}
