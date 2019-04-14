//
// Created by eraden on 09.02.19.
//

#pragma once

#include <string>
#include <sstream>
#include <memory>
#include <libpq-fe.h>

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
            logger->info(query->query());
            pg_result *res = PQexecParams(
                    conn,
                    query->query(), query->argc(),
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
    };
}
