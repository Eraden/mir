#include <string>
#include <iostream>

#include <mir/db/Connection.hpp>
#include <mir/db/Url.hpp>
#include <mir/utils/Logger.hpp>
#include <mir/utils/SqlPrinter.hpp>
#include <mir/mir/CommandLineInterface.hpp>
#include <mir/mir/UpMigrator.hpp>
#include <mir/mir/DownMigrator.hpp>
#include <mir/mir/SetupMigrator.hpp>
#include <mir/mir/CheckRunner.hpp>
#include <mir/mir/Generator.hpp>
#include <mir/mir/ConfigReader.hpp>

using namespace std;

int main(int argc, char **argv) {
    CommandLineInterface cli;

    for (int i = 0; i < argc; i++) {
        std::string v = argv[i];
        if (v == "test") {
            std::string sql = R"(INSERT INTO users(login, email) VALUES ($1, $2) RETURNING * ["Adi", "adi@example.com"])";
            SqlPrinter::SqlPrinter().call(sql);
            return 0;
        }

        if (v == "--help" || v == "-h") {
            cli.action = CommandLineInterface::Action::Help;
            break;
        } else if (v == "--url" && i + 2 < argc) {
            string url = argv[i + 1];

            cli.url = std::make_shared<Database::Url>();
            Database::UrlParser urlParser;
            urlParser.parse(cli.url, url);
            i += 1;
        } else if (v == "-v" || v == "--version") {
            cli.printVersion();
            std::exit(0);
        } else if (v == "check") {
            cli.action = CommandLineInterface::Action::Check;
        } else if (v == "up") {
            cli.action = CommandLineInterface::Action::MigrateUp;
        } else if (v == "down") {
            cli.action = CommandLineInterface::Action::MigrateDown;
        } else if (v == "setup") {
            cli.action = CommandLineInterface::Action::Setup;
        } else if (mir::Generator::match(v)) {
            cli.action = CommandLineInterface::Action::Generate;
        } else if (v == "-q" || v == "--quite") {
            cli.setQuite();
        }
    }
    ConfigReader configReader(&cli);
    configReader.run();

    if (cli.isDatabaseRequired() && !cli.url) {
        cerr << "No database url" << endl;
        return 1;
    }

    switch (cli.action) {
        case CommandLineInterface::Action::Setup: {
            SetupMigrator::SetupMigrator setupMigrator(&cli);
            setupMigrator.run();
            break;
        }
        case CommandLineInterface::Action::MigrateUp: {
            UpMigrator::UpMigrator upMigrator(&cli);
            upMigrator.run();
            break;
        }
        case CommandLineInterface::Action::MigrateDown: {
            DownMigrator::DownMigrator downMigrator(&cli);
            downMigrator.run();
            break;
        }
        case CommandLineInterface::Action::Help: {
            cli.printHelp();
            return 0;
        }
        case CommandLineInterface::Action::Generate: {
            mir::Generator generator(&cli, argc, argv);
            generator.run();
            break;
        }
        case CommandLineInterface::Action::Check: {
            CheckRunner::CheckRunner checkRunner(&cli);
            if (!checkRunner.run()) return 1;
            break;
        }
    }

    return 0;
}
