//
// Created by eraden on 09.02.19.
//

#pragma once

#include <memory>
#include <vector>
#include <libpq-fe.h>

namespace Database {
    template <class Model>
    class Query {
    public:
        static const int TEXT_FORMAT = 0;
        static const int BINARY_FORMAT = 1;

        explicit Query(unsigned type) : type(type) {}

        virtual const char *query() = 0;

        virtual const char **argv() = 0;

        virtual const int argc() = 0;

        virtual void fetchResult(PGresult *result) = 0;

        virtual std::vector<Model> getResult() = 0;

        inline const unsigned getType() const { return type; }

        virtual int getOutputFormat() const { return TEXT_FORMAT; }
    private:
        unsigned type;
    };
}
