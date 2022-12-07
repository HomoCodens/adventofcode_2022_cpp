#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "./days.hpp"

namespace day3 {
    using std::string;

    // 52 bits used to encode letters appearing in a backpack
    // lsb -> 'a', 52nd bit -> 'Z'
    using Digitmon = uint_fast64_t;

    namespace {
        // Map a-Z to 0-51
        int charToIndex(char c) {
            if(c >= 'a') {
                return c - 'a';
            } else {
                return c - 'A' + 26;
            }
        }

        // Digest a string into bits in a Digitmon
        Digitmon digitize(std::string::iterator start, std::string::iterator end) {
            Digitmon d{0};
            Digitmon oneMon{1};
            while(start != end) {
                d |= (oneMon << charToIndex(*(start++)));
            }
            return d;
        }

        // Find the one bit on the Digitmon that is 1 and return its 'score'
        int scoreDigitmon(Digitmon d) {
            if(d == 0) {
                return 0;
            }

            if(d == 1) {
                return 1;
            }

            int score{0};
            do {
                ++score;
            } while((d >>= 1) > 0);

            return score;
        }
    }

    DayResults run(std::string filename) {
        std::ifstream f{filename};

        int total{0};

        int groupTotal{0};
        int grpCounter{0};
        Digitmon group{0};
        
        string line{};
        
        while(!f.eof()) {
            std::getline(f, line);

            // For part 1, find the one bit in common between the two halves of line
            auto lngth{line.length()};
            auto bgn{line.begin()};
            
            Digitmon firstHalf{digitize(line.begin(), line.begin() + lngth/2)};
            Digitmon secondHalf{digitize(line.begin() + lngth/2, line.end())};
            
            total += scoreDigitmon(firstHalf & secondHalf);

            // For part 2, first 'concatenate' the two halves
            Digitmon bothHalves = firstHalf | secondHalf;

            // ...then for each group of 3, find what they have in common
            if(grpCounter == 0) {
                group = bothHalves;
            } else {
                group &= bothHalves;
                if(grpCounter == 2) {
                    groupTotal += scoreDigitmon(group);
                }
            }
            grpCounter = (grpCounter + 1) % 3;
        }

        return DayResults{
            "The scores of the misplaced items sum up to " + std::to_string(total),
            "The scores of the common items from all groups sum to " + std::to_string(groupTotal)
        };
    }
}