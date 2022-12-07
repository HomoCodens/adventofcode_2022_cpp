#pragma once

#include <chrono>
#include <functional>
#include <string>


struct DayResults {
    std::string part1{"Not implemented."};
    std::string part2{"Not implemented."};
};

std::ostream& operator<<(std::ostream& os, const DayResults& res);

using TimeUnit = std::chrono::microseconds;
struct DayWithTime {
    int dayNumber{};
    DayResults results{};
    TimeUnit t{};
};

std::ostream& operator<<(std::ostream& os, const DayWithTime& res);

using Runner = std::function<DayResults(std::string)>;
DayWithTime runAndTime(int day, Runner solver, std::string input, int nSamples = 100);
