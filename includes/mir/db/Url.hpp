//
// Created by eraden on 10.02.19.
//

#pragma once

#include <string>
#include <sstream>
#include <memory>

using namespace std;

namespace Database {
    class Url {
    public:
        Url() = default;
        ~Url() = default;

        string protocol;
        string host;
        string username;
        string password;
        string database;

        bool empty();

        const char *c_str();
    private:
        string str;
    };

    class UrlParser {
    public:
        bool parse(shared_ptr<Url> url, const string &str);
    };
}
