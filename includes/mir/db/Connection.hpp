//
// Created by eraden on 09.02.19.
//

#pragma once

#include <string>
#include <sstream>
#include <memory>
#include <libpq-fe.h>
#include <boost/utility/string_view.hpp>

#include <mir/utils/Logger.hpp>
#include <mir/db/Query.hpp>

using namespace std;

namespace Database {
    class Connection {
    public:
        explicit Connection(const char *connInfo, std::shared_ptr<Logger> logger);

        explicit Connection(const std::string connInfo, std::shared_ptr<Logger> logger);

        ~Connection();

        template<class Model>
        bool execute(Query<Model> *query) {
            bool success = false;
            std::string sql = prepareSql(std::string(query->query()));
            logger->info(sql);
            pg_result *res = PQexecParams(
                    conn,
                    sql.c_str(), query->argc(),
                    nullptr, /* let the backend deduce param type */
                    query->argv(),
                    nullptr, /* don't need param lengths since text */
                    nullptr, /* default to all text params */
                    query->getOutputFormat() /* ask for binary results */
            );

            auto status = PQresultStatus(res);
            if (isOk(status)) {
                query->fetchResult(res);
                success = true;
            } else {
                std::stringstream ss;
                std::string statusName = getStatusName(status);
                ss << "[" << statusName << "] Query failed: " << PQerrorMessage(conn);
                logger->error(ss.str());
            }
            PQclear(res);
            return success;
        }

        template<class Model>
        bool executeSql(Query<Model> *query) {
            bool success = false;
            logger->info(query->query());
            pg_result *res = PQexec(conn, query->query());

            auto status = PQresultStatus(res);
            if (isOk(status)) {
                query->fetchResult(res);
                success = true;
            } else {
                std::stringstream ss;
                std::string statusName = getStatusName(status);
                ss << "[" << statusName << "] Query failed: " << PQerrorMessage(conn);
                logger->error(ss.str());
            }
            PQclear(res);
            return success;
        }

    private:
        std::string connInfo;
        PGconn *conn = nullptr;
        std::shared_ptr<Logger> logger = nullptr;

        void exitNicely();

        std::string getStatusName(ExecStatusType status);

        bool isOk(ExecStatusType status);

        std::string prepareSql(std::string s) const {
            std::stringstream ss;
            std::string token;
            std::istringstream tokenStream(s);
            while (std::getline(tokenStream, token, '\n')) {
                ss << trim(token) << "\n";
            }
            s = ss.str();
            return s;
        }

	boost::string_view trim(boost::string_view s) const {
            s.remove_prefix(std::min(s.find_first_not_of(" \t\r\v\n"), s.size()));
            s.remove_suffix((s.size() - 1) - std::min(s.find_last_not_of(" \t\r\v\n"), s.size() - 1));

            return s;
        }
    };
}
