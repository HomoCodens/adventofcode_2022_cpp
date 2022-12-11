#include <iostream>
#include <fstream>
#include <functional>
#include <vector>
#include <algorithm>
#include <chrono>

#include "./src/util.hpp"
#include "./src/days.hpp"

int main() {
    
    std::vector<Runner> allTheDays{
        day1::run,
        day2::run,
        day3::run,
        day4::run,
        day5::run,
        day6::run,
        day7::run,
        day8::run,
        day9::run,
        day10::run,
        day11::run
    };

    TimeUnit totalTime{0};

    for(int i = 10; i < 11; i++) {
        int day{i+1};

        std::string dayStr{std::to_string(day)};
        if(day < 10) {
            dayStr = "0" + dayStr;
        }

        std::string path = "./input/day" + dayStr + "/input.txt";

        DayWithTime dt = runAndTime(day, allTheDays[i], path, 1);
        std::cout << dt;

        totalTime += dt.t;
    }

    std::cout << "Total time taken: " << totalTime << '\n';
    return 0;
}
