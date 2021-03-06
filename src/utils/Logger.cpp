//
// Created by eraden on 10.02.19.
//

#include <mir/utils/Logger.hpp>

void Logger::log(string msg) {
    std::time_t t = std::time(nullptr);
    std::tm *now = std::localtime(&t);

    if (severity >= Severity::LOG)
        send(msg, Severity::LOG, now, programName);
}

void Logger::info(string msg) {
    std::time_t t = std::time(nullptr);
    std::tm *now = std::localtime(&t);

    if (severity >= Severity::INFO)
        send(msg, Severity::INFO, now, programName);
}

void Logger::warn(string msg) {
    std::time_t t = std::time(nullptr);
    std::tm *now = std::localtime(&t);

    if (severity >= Severity::WARN)
        send(msg, Severity::WARN, now, programName);
}

void Logger::error(string msg) {
    std::time_t t = std::time(nullptr);
    std::tm *now = std::localtime(&t);

    if (severity >= Severity::ERROR)
        send(msg, Severity::ERROR, now, programName);
}

void Logger::send(const std::string &msg, Logger::Severity severity, std::tm *timestamp, const std::string &programName) {
    if (muted) return;
    for (auto &out : outputs) {
        out->receive(msg, severity, timestamp, programName);
    }
}

void StandardConsoleOutput::receive(
        const std::string &msg,
        Logger::Severity severity,
        std::tm *timestamp,
        const std::string &programName
) {
    std::stringstream ss;
    std::stringstream mss;
    for (const auto &c : msg) {
        if (c == '\n') mss << "\\n";
        else if (c == '\t') mss << "\\t";
        else if (c == '\r') mss << "\\r";
        else mss << c;
    }

    char time[1024];
    strftime(time, 1024, "%Y-%m-%dT%H:%M:%S", timestamp);
    std::string severityName;
    switch (severity) {
        case Logger::LOG:
            severityName = severityName + COLOR_BLUE + "LOG" + COLOR_NC;
            break;
        case Logger::INFO:
            severityName = severityName + COLOR_GREEN + "INFO" + COLOR_NC;
            break;
        case Logger::WARN:
            severityName = severityName + COLOR_YELLOW + "WARN" + COLOR_NC;
            break;
        case Logger::ERROR:
            severityName = severityName + COLOR_RED + "ERROR" + COLOR_NC;
            break;
    }

    ss << "[" << severityName << "] " << "[" << programName << "][" << time << "] " << mss.rdbuf() << std::endl;

    switch (severity) {
        case Logger::LOG:;
        case Logger::INFO: {
            std::cout << ss.str();
            break;
        }
        case Logger::WARN:
        case Logger::ERROR: {
            std::cerr << ss.str();
            break;
        }
    }
}
