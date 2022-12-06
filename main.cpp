#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

#include "./src/util.hpp"
#include "./src/days.hpp"

int main() {
    
    DayWithTime dt = runAndTime(6, day6::run, "./input/day06/input.txt");
    std::cout << dt << '\n';

    return 0;
}
