//
// Created by eraden on 09.02.19.
//

#include "CommandLineInterface.hpp"

const char *CommandLineInterface::HELP = ""
                                         "mir - migrate your database\n"
                                         "    --help | -h               display this message\n"
                                         "    --url                     database url\n"
                                         "    up                        run migrations from last saved point\n"
                                         "    down                      reverse last migration\n"
                                         "    setup [database name]     create database";

CommandLineInterface::CommandLineInterface() :
        action(Action::Help),
        url(nullptr),
        logger(nullptr) {
    logger = std::make_shared<Logger>("mir");
    logger->setSeverity(Logger::Severity::ERROR);
    logger->addReceiver(std::make_shared<StandardConsoleOutput>());
}

void CommandLineInterface::printHelp() {
    std::cout << HELP << std::endl;
}

fs::path CommandLineInterface::getMigrationsDir() {
    if (migrationsDir == nullptr) {
        auto pwd = fs::current_path();
         migrationsDir = std::make_shared<fs::path>(fs::absolute(pwd / "db" / "migrations"));
    }
    return *migrationsDir;
}

void CommandLineInterface::changeMigrationsDir(const boost::filesystem::path &path) {
    migrationsDir = make_shared<fs::path>(path);
}

bool CommandLineInterface::isDatabaseRequired() {
    return !(action == Action::Help || action == Action::Generate);
}
