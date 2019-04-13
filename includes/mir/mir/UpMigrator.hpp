//
// Created by eraden on 10.02.19.
//

#pragma once

#include <sstream>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

#include <mir/db/Connection.hpp>
#include <mir/db/Url.hpp>

class CommandLineInterface;

namespace UpMigrator {
    namespace fs = boost::filesystem;

    class UpMigrator {
    public:
        explicit UpMigrator(CommandLineInterface *cli);

        bool run();

    private:
        CommandLineInterface *cli = nullptr;
        std::shared_ptr<Database::Url> url = nullptr;
        std::unique_ptr<Database::Connection> connection = nullptr;
    };
}
