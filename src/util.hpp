#pragma once

#include <string>

struct DayResults {
    std::string part1{};
    std::string part2{};
};

std::ostream& operator<<(std::ostream& os, const DayResults& res);