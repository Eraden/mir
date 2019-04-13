//
// Created by eraden on 09.02.19.
//

#pragma once

#include <string>
#include <iostream>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <mir/db/Connection.hpp>
#include <mir/db/Url.hpp>
#include <mir/utils/Logger.hpp>
#include <mir/mir/CommandLineInterface.hpp>
#include <mir/mir/UpMigrator.hpp>
#include <mir/mir/SetupMigrator.hpp>

using namespace std;
namespace fs = boost::filesystem;

class CommandLineInterface {
public:
    enum struct Action {
        Setup,
        MigrateUp,
        MigrateDown,
        Help,
        Generate
    };

    CommandLineInterface();

    Action action;
    std::shared_ptr<Database::Url> url = nullptr;
    std::shared_ptr<Logger> logger = nullptr;
    std::string defaultType = "text";
    bool putTimestamps = false;
    std::vector<std::string> primaryKeys;

    void printHelp();

    fs::path getMigrationsDir();

    void changeMigrationsDir(const boost::filesystem::path &path);

    bool isDatabaseRequired();
private:
    static const char *HELP;
    std::shared_ptr<boost::filesystem::path> migrationsDir = nullptr;
};
