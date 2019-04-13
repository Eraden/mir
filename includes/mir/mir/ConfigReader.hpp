//
// Created by eraden on 10.02.19.
//

#pragma once

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

class CommandLineInterface;

class ConfigReader {
public:
    explicit ConfigReader(CommandLineInterface *cli);

    void run();

private:
    CommandLineInterface *cli = nullptr;
};
