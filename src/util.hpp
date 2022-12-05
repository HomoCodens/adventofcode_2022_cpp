#pragma once

#include <chrono>
#include <string>

using TimeUnit = std::chrono::microseconds;

struct DayResults {
    std::string part1{};
    std::string part2{};
};

std::ostream& operator<<(std::ostream& os, const DayResults& res);

struct DayWithTime {
    DayResults results;
    TimeUnit t;
};