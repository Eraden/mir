//
// Created by eraden on 10.02.19.
//

#pragma once

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

class CommandLineInterface;

namespace MigrationDirectoryReader {
    namespace fs = boost::filesystem;

    class MigrationDirectory {
    public:
        explicit MigrationDirectory(const std::string &name, const fs::path &path, unsigned long migration);

        std::string name;
        fs::path path;
        unsigned long migration;
    };

    std::vector<MigrationDirectory> readMigrationDirectory(CommandLineInterface *cli);
}
