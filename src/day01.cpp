#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include "./day01.hpp"
#include "./util.hpp"

namespace day1 {
    DayResults run(std::string filename) {
        std::ifstream f{ filename };

        int current{};
        std::vector<int> elfs{};

        while(!f.eof()) {
            std::string l{};
            std::getline(f, l);
            if(l == "") {
                elfs.push_back(current);
                current = 0;
            } else {
                current += std::atoi(l.c_str());
            }
        }

        std::sort(elfs.begin(), elfs.end(), std::greater{});

        return DayResults{
            "The fattest elf is carrying " + std::to_string(elfs[0]) + " calories.",
            "The three most burdened elfs carry " + std::to_string(elfs[0] + elfs[1] + elfs[2]) + " calories."
        };
    }
}