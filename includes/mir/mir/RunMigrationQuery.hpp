//
// Created by eraden on 10.02.19.
//

#pragma once

#include <mir/db/Query.hpp>

class RunMigrationQuery : public Database::Query<int> {
public:
    explicit RunMigrationQuery(const std::string &contents);

    const char *query() override;

    const char **argv() override;

    const int argc() override;

    void fetchResult(PGresult *result) override;

    std::vector<int> getResult() override;
private:
    const std::string contents;
};
