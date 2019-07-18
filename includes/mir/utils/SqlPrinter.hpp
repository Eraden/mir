//
// Created by eraden on 18.07.19.
//

#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <mir/utils/SqlPrinterData.hpp>

namespace SqlPrinter {
    class SqlPrinter {
    public:
        static std::string call(const std::string &sql);

    private:
        static std::vector<std::string> tokenize(const std::string &sql);

        static std::vector<std::string> preprocess(std::vector<std::string> &old);

        static void preprocessToken(const std::string token, std::vector<std::string> &tokens);

        static std::string highlightToken(const std::string &token);

        static bool isOperator(const std::string &token);

        static bool isKeyword(const std::string &token);

        static bool isValue(const std::string &token);

        static bool isNumber(const std::string &token);

        static bool isWhiteJoinRequired(int pos, const std::vector<std::string> &tokens);
    };
}
