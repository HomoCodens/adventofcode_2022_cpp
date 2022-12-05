#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "./day03.hpp"

namespace day3 {
    using std::string;

    int scoreChar(char c) {
        if(c >= 'a') {
            return c - 'a' + 1;
        } else {
            return c - 'A' + 1 + 26;
        }
    }

    int analyzeBackpack(string bp) {
        auto lngth{bp.length()};
        auto bgn{bp.begin()};
        
        // Sort the first and second half of the backpack independently
        std::sort(bgn, bgn + lngth/2);
        std::sort(bgn + lngth/2, bp.end());

        // find the set intersection between the two halves
        string offender{};
        std::set_intersection(bgn, bgn + lngth/2, bgn + lngth/2, bp.end(), std::back_inserter(offender));

        return scoreChar(offender[0]);
    }

    int badgeGroup(std::vector<string> group) {
        for(string& elf : group) {
            std::sort(elf.begin(), elf.end());
        }

        string ab{};
        string bc{};

        std::set_intersection(group[0].begin(), group[0].end(), group[1].begin(), group[1].end(), std::back_inserter(ab));
        std::set_intersection(group[1].begin(), group[1].end(), group[2].begin(), group[2].end(), std::back_inserter(bc));

        string abc{};
        std::set_intersection(ab.begin(), ab.end(), bc.begin(), bc.end(), std::back_inserter(abc));

        return scoreChar(abc[0]);
    }

    void run(std::string filename) {
        std::ifstream f{filename};

        int total{0};
        int badgeTotal{};
        string line{};
        std::vector<string> activeGroup;
        while(!f.eof()) {
            std::getline(f, line);
            if(line != "") {
                total += analyzeBackpack(line);

                activeGroup.push_back(line);
                if(activeGroup.size() == 3) {
                    badgeTotal += badgeGroup(activeGroup);
                    activeGroup.clear();
                }
            }
        }

        std::cout << total << '\n';
        std::cout << badgeTotal << '\n';
    }
}