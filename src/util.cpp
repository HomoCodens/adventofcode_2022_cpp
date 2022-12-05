#include <iostream>

#include "./util.hpp"

std::ostream& operator<<(std::ostream& os, const DayResults& res) {
    os << "Part 1:\n" << res.part1 << '\n';
    os << "Part 2:\n" << res.part2 << '\n';

    return os;
}