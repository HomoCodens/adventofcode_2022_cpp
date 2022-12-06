#pragma once

#include <chrono>
#include <functional>
#include <string>

using TimeUnit = std::chrono::microseconds;

struct DayResults {
    std::string part1{"Not implemented."};
    std::string part2{"Not implemented."};
};

std::ostream& operator<<(std::ostream& os, const DayResults& res);

struct DayWithTime {
    int dayNumber{};
    DayResults results{};
    TimeUnit t{};
};

std::ostream& operator<<(std::ostream& os, const DayWithTime& res);

DayWithTime runAndTime(int day, std::function<DayResults(std::string)> solver, std::string input, int nSamples = 100);
