//
// Created by eraden on 09.02.19.
//

#include <db/Connection.hpp>

namespace Database {
    Connection::Connection(const char *connInfo, std::shared_ptr<Logger> logger) :
            logger(std::move(logger)) {
        conn = PQconnectdb(connInfo);

        if (PQstatus(conn) != CONNECTION_OK) {
            fprintf(stderr, "Connection to db failed: %s", PQerrorMessage(conn));
            exitNicely();
        }
    }

    Connection::~Connection() {
        exitNicely();
    }

    void Connection::exitNicely() {
        PQfinish(conn);
    }

    std::string Connection::getStatusName(ExecStatusType status) {
        switch (status) {
            case PGRES_EMPTY_QUERY:
                return "PGRES_EMPTY_QUERY";
            case PGRES_COPY_OUT:
                return "PGRES_COPY_OUT";
            case PGRES_COPY_IN:
                return "PGRES_COPY_IN";
            case PGRES_BAD_RESPONSE:
                return "PGRES_BAD_RESPONSE";
            case PGRES_NONFATAL_ERROR:
                return "PGRES_NONFATAL_ERROR";
            case PGRES_FATAL_ERROR:
                return "PGRES_FATAL_ERROR";
            case PGRES_COPY_BOTH:
                return "PGRES_COPY_BOTH";
            case PGRES_SINGLE_TUPLE:
                return "PGRES_SINGLE_TUPLE";
            default:
                return "";
        }
    }

    bool Connection::isOk(ExecStatusType status) {
        return status == PGRES_COMMAND_OK || status == PGRES_TUPLES_OK;
    }
}
