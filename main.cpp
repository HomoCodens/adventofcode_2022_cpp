#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

#include "./src/util.hpp"
#include "./src/days.hpp"

int main() {
    auto start = std::chrono::steady_clock::now();
    
    DayResults d1{day1::run("./input/day01/input.txt")};
    std::cout << d1;
    DayResults d2{day2::run("./input/day02/input.txt")};
    std::cout << d2;
    DayResults d3{day3::run("./input/day03/input.txt")};
    std::cout << d3;
    DayResults d4{day4::run("./input/day04/input.txt")};
    std::cout << d4;
    DayResults d5{day5::run("./input/day05/input.txt")};
    std::cout << d5;

    auto end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time: "
         << std::chrono::duration_cast<TimeUnit>(end - start) << std::endl;

    return 0;
}
