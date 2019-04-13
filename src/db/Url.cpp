//
// Created by eraden on 10.02.19.
//

#include <mir/db/Url.hpp>

#include <iostream>

namespace Database {
    bool UrlParser::parse(shared_ptr<Url> url, const string &str) {
        auto rest = string(str);
        auto pos = rest.find_first_of(':');
        if (pos == string::npos) {
            return false;
        }
        url->protocol = rest.substr(0, pos);
        pos = rest.find_first_of('/', pos + 2);

        if (pos == string::npos) {
            return false;
        }
        rest = rest.substr(pos + 1);

        pos = rest.find_first_of('@');
        if (pos != string::npos && rest[pos - 1] != '\\') {
            pos = rest.find_first_of(':');
            if (pos == string::npos) {
                pos = rest.find_first_of('@');
                url->username = rest.substr(0, pos);
                rest = rest.substr(pos + 1);
            } else {
                url->username = rest.substr(0, pos);
                rest = rest.substr(pos + 1);
                pos = rest.find_first_of('@');
                url->password = rest.substr(0, pos);
                rest = rest.substr(pos + 1);
            }
        }

        pos = rest.find_first_of('/');
        if (pos == string::npos) {
            return false;
        }
        url->host = rest.substr(0, pos);
        rest = rest.substr(pos + 1);
        url->database = rest;

        return true;
    }

    bool Url::empty() {
        return protocol.empty() && host.empty() && username.empty() && password.empty() && database.empty();
    }

    const char *Url::c_str() {
        std::stringstream ss;
        ss << protocol << "://";
        if (!username.empty()) {
            ss << username;
            if (!password.empty()) {
                ss << ":" << password;
            }
            ss << "@";
        }
        ss << host << "/" << database;
        str = ss.str();

        return str.c_str();
    }
}

#ifdef UNIT_TESTING

#include <boost/test/unit_test_suite.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_CASE(database_url_parser_parse_simple_url) {
    auto url = make_shared<Database::Url>();
    Database::UrlParser parser;
    string str = "postgres://localhost/foo";

    BOOST_TEST(parser.parse(url, str));
    BOOST_REQUIRE_EQUAL(url->protocol, "postgres");
    BOOST_REQUIRE_EQUAL(url->username, "");
    BOOST_REQUIRE_EQUAL(url->password, "");
    BOOST_REQUIRE_EQUAL(url->host, "localhost");
    BOOST_REQUIRE_EQUAL(url->database, "foo");
}

BOOST_AUTO_TEST_CASE(database_url_parser_parse_url_with_username) {
    auto url = make_shared<Database::Url>();
    Database::UrlParser parser;
    string str = "postgres://user@localhost/foo";
    parser.parse(url, str);

    BOOST_TEST(parser.parse(url, str));
    BOOST_REQUIRE_EQUAL(url->protocol, "postgres");
    BOOST_REQUIRE_EQUAL(url->username, "user");
    BOOST_REQUIRE_EQUAL(url->password, "");
    BOOST_REQUIRE_EQUAL(url->host, "localhost");
    BOOST_REQUIRE_EQUAL(url->database, "foo");
}

BOOST_AUTO_TEST_CASE(database_url_parser_parse_url_with_creds) {
    auto url = make_shared<Database::Url>();
    Database::UrlParser parser;
    string str = "postgres://user:pass@localhost/foo";
    parser.parse(url, str);

    BOOST_TEST(parser.parse(url, str));
    BOOST_REQUIRE_EQUAL(url->protocol, "postgres");
    BOOST_REQUIRE_EQUAL(url->username, "user");
    BOOST_REQUIRE_EQUAL(url->password, "pass");
    BOOST_REQUIRE_EQUAL(url->host, "localhost");
    BOOST_REQUIRE_EQUAL(url->database, "foo");
}

#endif
