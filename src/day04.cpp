#include <fstream>
#include <iostream>
#include <string>

#include "./days.hpp"

namespace day4 {
    using std::string;

    namespace {
        struct range {
            int min{};
            int max{};
        };

        bool hazNommed(const range& nommer, const range& nommee) {
            return nommer.min <= nommee.min && nommer.max >= nommee.max;
        }

        bool hasOverlab(const range& r1, const range& r2) {
            return (r1.max >= r2.min && r1.min <= r2.min) ||
                    (r2.max >= r1.min && r2.min <= r1.min);
        }
    }

    DayResults run(string filename) {
        //auto f = fopen(filename.c_str(), "r");

        std::ifstream f{filename};

        int r1l{};
        int r1h{};
        int r2l{};
        int r2h{};
        char sep{};

        int part1{0};
        int part2{0};
        /*while(!feof(f)) {
            std::fscanf(f, "%d-%d,%d-%d", &a, &b, &c, &d);
            range r1{a, b};
            range r2{c, d};

            // Adding bools just never gets old
            part1 += (hazNommed(r1, r2) || hazNommed(r2, r1));
            part2 += (hasOverlab(r1, r2) || hasOverlab(r2, r1));
        }

        fclose(f);*/

        while(!f.eof()) {
            sep = '\0';
            f >> r1l >> sep >> r1h;
            f >> sep >> r2l >> sep >> r2h;
            if(sep == '-') {
                range r1{r1l, r1h};
                range r2{r2l, r2h};

                // Adding bools just never gets old
                part1 += (hazNommed(r1, r2) || hazNommed(r2, r1));
                part2 += hasOverlab(r1, r2);
            }
        }

        return DayResults {
            "There are " + std::to_string(part1) + " teams where one range completely overlaps the other.",
            "There are " + std::to_string(part2) + " teams with some overlap."
        };
    }
}