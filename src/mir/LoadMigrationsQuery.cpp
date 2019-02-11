//
// Created by eraden on 10.02.19.
//

#include "LoadMigrationsQuery.hpp"

const char *LoadMigrationsQuery::SELECT_MIGRATIONS = R"(SELECT id, migration FROM _mir_migrations ORDER BY migration;)";

const char *LoadMigrationsQuery::query() {
    return SELECT_MIGRATIONS;
}

const char **LoadMigrationsQuery::argv() {
    return nullptr;
}

std::vector<Migration> LoadMigrationsQuery::getResult() {
    return rows;
}

void LoadMigrationsQuery::fetchResult(PGresult *result) {
    std::vector<Migration> rows;
    auto idField = PQfnumber(result, "id");
    auto migrationField = PQfnumber(result, "migration");
    auto len = PQntuples(result);
    rows.resize(static_cast<unsigned long>(len));
    for (auto i = 0; i < len; i++) {
        auto &model = rows[i];
        if (!PQgetisnull(result, i, idField)) {
            auto id = PQgetvalue(result, i, idField);
            model.id = std::strtoul(id, nullptr, 0);
        }

        if (!PQgetisnull(result, i, migrationField)) {
            auto migration = PQgetvalue(result, i, migrationField);
            model.migration = std::strtoul(migration, nullptr, 0);
        }
    }
    this->rows = rows;
}

const int LoadMigrationsQuery::argc() {
    return 0;
}

#ifdef UNIT_TESTING

#include <boost/test/unit_test_suite.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_CASE(mir_load_migrations_test) {
    LoadMigrationsQuery query;

    BOOST_REQUIRE_EQUAL(query.argv(), nullptr);
    BOOST_REQUIRE_EQUAL(query.argc(), 0);
}
#endif
