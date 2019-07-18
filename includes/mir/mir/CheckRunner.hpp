//
// Created by eraden on 17.07.19.
//

#pragma once

#include "CommandLineInterface.hpp"
#include <mir/mir/LoadMigrationsQuery.hpp>
#include <mir/mir/MigrationDirectoryReader.hpp>

namespace CheckRunner {
    class CheckRunner {
    public:
        explicit CheckRunner(CommandLineInterface *cli);

        bool run();

    private:
        CommandLineInterface *cli = nullptr;
        std::shared_ptr <Database::Url> url = nullptr;
        std::unique_ptr <Database::Connection> connection = nullptr;
    };;
}
