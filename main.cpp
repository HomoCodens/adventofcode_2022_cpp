#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

#include "./src/util.hpp"
#include "./src/days.hpp"

int main() {
    
    DayWithTime dt = runAndTime(1, day1::run, "./input/day01/input.txt");
    std::cout << dt << '\n';

    return 0;
}
