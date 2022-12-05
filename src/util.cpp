#include <iostream>

#include "./util.hpp"

std::ostream& operator<<(std::ostream& os, const DayResults& res) {
    os << "Part 1:\n" << res.part1 << '\n';
    os << "Part 2:\n" << res.part2 << '\n';

    return os;
}

std::ostream& operator<<(std::ostream& os, const DayWithTime& res) {
    os << "============\n" << "   Day " << std::to_string(res.dayNumber) << "\n============\n\n";
    os << res.results << '\n';
    os << "Time taken: " << res.t << "\n\n";

    return os;
}

DayWithTime runAndTime(int day,
                        std::function<DayResults(std::string)> solver,
                        std::string input,
                        int nSamples) {
    TimeUnit totalTime{0};
    DayResults solvolution{};
    for(int i = 0; i < nSamples; i++) {
        auto start = std::chrono::steady_clock::now();
        solvolution = solver(input);
        auto end = std::chrono::steady_clock::now();
        totalTime += std::chrono::duration_cast<TimeUnit>(end - start);
    }

    return DayWithTime{
        day,
        solvolution,
        totalTime / nSamples
    };
}
