//
// Created by eraden on 09.02.19.
//

#pragma once

#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

namespace Database {
    struct Stat {
        unsigned id;
        wstring url;
        wstring client;
        time_point createdAt;
    };
}
