//
// Created by eraden on 10.02.19.
//

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <memory>

using namespace std;

class Output;

class Logger {
public:
    enum Severity {
        LOG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4
    };

    explicit Logger(const char *programName) : programName(programName), severity(Severity::ERROR) {}
    explicit Logger(std::string programName) : programName(programName), severity(Severity::ERROR) {}

    void log(string msg);

    void info(string msg);

    void warn(string msg);

    void error(string msg);

    inline void setSeverity(Severity severity) { this->severity = severity; }

    inline void addReceiver(const shared_ptr<Output> &output) { outputs.push_back(output); }
private:
    std::string programName;
    Severity severity;
    std::vector<shared_ptr<Output>> outputs;

    void send(const std::string &msg, Severity severity, std::tm *timestamp, const std::string &programNam);
};

class Output {
public:
    virtual void receive(const std::string &msg, Logger::Severity severity, std::tm *timestamp, const std::string &programNam) = 0;
};

class StandardConsoleOutput : public Output {
public:
    virtual ~StandardConsoleOutput() = default;
    void receive(const std::string &msg, Logger::Severity severity, std::tm *timestamp, const std::string &programNam) override;
};
