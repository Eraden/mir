//
// Created by eraden on 10.02.19.
//

#include <mir/mir/RunMigrationQuery.hpp>

using namespace std;

RunMigrationQuery::RunMigrationQuery(const std::string &contents) :
        Database::Query<int>(2),
        contents(contents) {
}

const char **RunMigrationQuery::argv() {
    return nullptr;
}

const char *RunMigrationQuery::query() {
    return contents.c_str();
}

const int RunMigrationQuery::argc() {
    return 0;
}

void RunMigrationQuery::fetchResult(PGresult *result) {
}

std::vector<int> RunMigrationQuery::getResult() {
    vector<int> v;
    return v;
}
