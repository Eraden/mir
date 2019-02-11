//
// Created by eraden on 10.02.19.
//

#pragma once

#include <libpq-fe.h>
#include <db/Query.hpp>

struct Migration {
    unsigned long id;
    unsigned long migration;
};

class LoadMigrationsQuery : public Database::Query<Migration> {
public:
    LoadMigrationsQuery() : Database::Query<Migration>(1) {}

    const char *query() override;

    const char **argv() override;

    const int argc() override;

    void fetchResult(PGresult *result) override;

    std::vector<Migration> getResult() override;

private:
    static const char *SELECT_MIGRATIONS;
    std::vector<Migration> rows;
};
