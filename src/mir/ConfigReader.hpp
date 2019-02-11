//
// Created by eraden on 10.02.19.
//

#pragma once

#include <boost/filesystem.hpp>

class CommandLineInterface;

class ConfigReader {
public:
    explicit ConfigReader(CommandLineInterface *cli);

    void run();

private:
    CommandLineInterface *cli = nullptr;
};
