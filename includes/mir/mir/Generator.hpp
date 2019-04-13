//
// Created by eraden on 11.02.19.
//

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

class CommandLineInterface;
namespace mir {

    class ColumnSpecification {
    public:
        explicit ColumnSpecification(std::string name, std::string type) :
                name(std::move(name)),
                type(std::move(type)) {
        }

        std::string name;
        std::string type;
        std::string defaultValue;
    };

    class TableGenerator {
    public:
        explicit TableGenerator(
                CommandLineInterface *cli,
                const std::string &tableName,
                const std::vector<std::string> &fields
        );

        void run();

    private:
        const std::string tableName;
        std::vector<std::shared_ptr<ColumnSpecification>> fields;
        CommandLineInterface *cli;

        void parseColumn(const std::string &columnSpec);

        bool isPrimaryKey(const std::shared_ptr<ColumnSpecification> &field) const;
    };

    class Generator {
    public:
        static bool match(const std::string &value);

        explicit Generator(CommandLineInterface *cli, int argc, char **argv);

        void run();

    private:
        std::vector<std::string> argv;
        CommandLineInterface *cli;
    };
}
