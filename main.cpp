#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

#include "./src/util.hpp"
#include "./src/day01.hpp"
#include "./src/day02.hpp"
#include "./src/day03.hpp"
#include "./src/day04.hpp"
#include "./src/day05.hpp"

int main() {
    auto start = std::chrono::steady_clock::now();
    
    DayResults d1{day1::run("./input/day01/input.txt")};
    std::cout << d1;
    /*day2::run("./input/day02/input.txt");
    day3::run("./input/day03/input.txt");
    day4::run("./input/day04/input.txt");
    day5::run("./input/day05/input.txt");*/

    auto end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time in microseconds: "
         << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
         << " µs" << std::endl;

    return 0;
}
