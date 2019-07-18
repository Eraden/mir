//
// Created by eraden on 18.07.19.
//

#include <mir/utils/SqlPrinter.hpp>
#include <iostream>

namespace SqlPrinter {
    std::string SqlPrinter::call(const std::string &sql) {
        auto tokens = tokenize(sql);
        tokens = preprocess(tokens);
        std::stringstream output;
        for (auto i = 0; i < tokens.size(); i++) {
            const auto t = tokens.at(i);
            const auto separator = isWhiteJoinRequired(i, tokens) ? " " : "";
            output << highlightToken(t) << separator;
        }
        output << COLOR_NC;
        return output.str();
    }

    std::vector<std::string> SqlPrinter::tokenize(const std::string &sql) {
        std::vector<std::string> tokens;

        unsigned long head = 0, tail = 0;
        while (true) {
            auto idx = sql.find_first_of("\t\n \r", head);
            if (idx == std::string::npos) {
                auto len = tail - 1 - head;
                const auto piece = sql.substr(head, len);
                if (!piece.empty()) tokens.push_back(piece);
                break;
            }
            if (tail != head) {
                auto len = tail - 1 - head;
                const auto piece = sql.substr(head, len);
                if (!piece.empty()) tokens.push_back(piece);
            }
            head = tail;
            tail = idx + 1;
        }
        return tokens;
    }

    std::vector<std::string> SqlPrinter::preprocess(std::vector<std::string> &old) {
        std::vector<std::string> tokens;
        for (const auto token : old) {
            preprocessToken(token, tokens);
        }
        return tokens;
    }

    void SqlPrinter::preprocessToken(const std::string token, std::vector<std::string> &tokens) {
        if (token.empty())
            return;
        for (const auto t : "(),[]") {
            if (token.size() == 1) continue;
            auto pos = token.find_first_of(t);
            if (pos == std::string::npos) continue;
            const auto head = token.substr(0, pos);
            if (!head.empty()) preprocessToken(head, tokens);
            auto matched =  std::string("") + t;
            preprocessToken(matched, tokens);
            const auto tail = token.substr(pos + 1);
            if (!tail.empty()) preprocessToken(tail, tokens);
            return;
        }
        tokens.push_back(token);
    }

    std::string SqlPrinter::highlightToken(const std::string &token) {
        if (isNumber(token)) return COLOR_CYAN + token;
        if (isOperator(token)) return COLOR_RED + token;
        if (isKeyword(token)) return COLOR_PURPLE + token;
        if (isValue(token)) return COLOR_BLUE + token;
        return COLOR_YELLOW + token;
    }

    bool SqlPrinter::isOperator(const std::string &token) {
        for (const auto &t : OPERATORS)
            if (token == t) return true;
        return false;
    }

    bool SqlPrinter::isKeyword(const std::string &token) {
        for (const auto &t : KEYWORDS)
            if (token == t) return true;
        return false;
    }

    bool SqlPrinter::isValue(const std::string &token) {
        return false;
    }

    bool SqlPrinter::isNumber(const std::string &token) {
        for (const auto &c : token)
            if (!std::isdigit(c))
                return false;
        return true;
    }

    bool SqlPrinter::isWhiteJoinRequired(int pos, const std::vector<std::string> &tokens) {
        if (pos + 1 >= tokens.size()) return false;
        const auto &current = tokens.at(pos);
        if (current == "(")
            return false;
        const auto &next = tokens.at(pos + 1);
        return next != "," && next != ")" && next != ";";
    }
}

