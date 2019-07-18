//
// Created by eraden on 09.02.19.
//

#include <mir/mir/CommandLineInterface.hpp>

const char *CommandLineInterface::VERSION = "mir 1.0.0";

const char *CommandLineInterface::HELP = R"(mir - migrate your database
    --help | -h                                       display this message
    --url                                             database url
    up                                                run migrations from last saved point
    down                                              reverse last migration
    setup database_name                               create database
    gen table table_name [field1:type1:default1]      generate migration)";

CommandLineInterface::CommandLineInterface() :
        action(Action::Help),
        url(nullptr),
        logger(nullptr) {
    logger = std::make_shared<Logger>("mir");
    logger->setSeverity(Logger::Severity::ERROR);
    logger->addReceiver(std::make_shared<StandardConsoleOutput>());
    url = std::make_shared<Database::Url>();
}

void CommandLineInterface::printHelp() {
    std::cout << HELP << std::endl;
}

void CommandLineInterface::printVersion() {
    std::cout << VERSION << std::endl;
}

fs::path CommandLineInterface::getMigrationsDir() {
    if (migrationsDir == nullptr) {
        auto pwd = fs::current_path();
        migrationsDir = std::make_shared<fs::path>(fs::absolute(pwd / getRelativeMigrationDir()));
    }
    return *migrationsDir;
}

fs::path CommandLineInterface::getRelativeMigrationDir() {
    if (relativeMigrationsDir == nullptr) {
        relativeMigrationsDir = std::make_shared<fs::path>(fs::path("db") / "migrations");
    }
    return *relativeMigrationsDir;
}

void CommandLineInterface::changeMigrationsDir(const boost::filesystem::path &path) {
    migrationsDir = make_shared<fs::path>(path);
}

bool CommandLineInterface::isDatabaseRequired() {
    return !(action == Action::Help || action == Action::Generate);
}

void CommandLineInterface::setQuite() {
    logger->mute();
}
