//
// Created by eraden on 10.02.19.
//

#pragma once

#include <cstring>
#include <string>
#include <memory>
#include <cstdlib>
#include <functional>
#include <utility>
#include <boost/filesystem.hpp>
#include <db/Connection.hpp>
#include <db/Url.hpp>

class CommandLineInterface;

namespace DownMigrator {
    namespace fs = boost::filesystem;

    class DownMigrator {
    public:
        explicit DownMigrator(CommandLineInterface *cli);

        bool run();

    private:
        CommandLineInterface *cli = nullptr;
        std::shared_ptr<Database::Url> url = nullptr;
        std::unique_ptr<Database::Connection> connection = nullptr;
    };
}
