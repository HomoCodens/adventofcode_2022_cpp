#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

#include "./src/util.hpp"
#include "./src/days.hpp"

int main() {
    
    DayWithTime dt = runAndTime(7, day7::run, "./input/day07/input.txt");
    std::cout << dt << '\n';

    return 0;
}
