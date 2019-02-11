//
// Created by eraden on 10.02.19.
//

#pragma once

#include <boost/filesystem.hpp>

class CommandLineInterface;

namespace MigrationDirectoryReader {
    namespace fs = boost::filesystem;

    class MigrationDirectory {
    public:
        explicit MigrationDirectory(const std::string &name, const fs::path &path, long migration);

        std::string name;
        fs::path path;
        long migration;
    };

    std::vector<MigrationDirectory> readMigrationDirectory(CommandLineInterface *cli);
};
