#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "./days.hpp"
#include "./util.hpp"

namespace day10 {
    using std::string;

    DayResults run(string filename) {
        std::ifstream f{filename};

        string instruction;
        int y;
        int x{1};
        int cycle{0};
        int signalStrength{0};
        string screen(240, '.');

        string line;
        while(!f.eof()) {
            instruction = "";
            y = 0;
            std::getline(f, line);

            // TODO: Why does f >> instruction >> y; crap out after the first noop??
            std::istringstream is{line};
            is >> instruction >> y;
            std::cout << "Instraction be " << instruction << ' ' << y << '\n';
            
            if(line != "") {
                if(instruction == "noop") {
                    // part 2
                    if(std::abs((cycle % 40) - x) <= 1) {
                        screen[cycle] = '#';
                    }

                    ++cycle;

                    // part 1 shtoff
                    if((cycle - 20) % 40 == 0) {
                        signalStrength += cycle*x;
                    }

                } else {
                    // part2
                    for(int i = 0; i < 2; ++i) {
                        if(std::abs(((cycle + i) % 40) - x) <= 1) {
                            screen[cycle + i] = '#';
                        }
                    }

                    cycle += 2;
                    x += y;

                    // part1
                    if((cycle - 20) % 40 == 0) {
                        // x is still on the previous value DURING the cycle
                        signalStrength += cycle*(x - y);
                    } else if((cycle - 21) % 40 == 0) {
                        // We overshot the counting cycle
                        signalStrength += (cycle - 1)*(x - y);
                    }
                }
            }
        }

        std::ostringstream os{};
        for(int i = 0; i < 240; ++i) {
            if(i > 0 && (i % 40) == 0) {
                os << '\n';
            }
            os << screen[i];
        }

        return DayResults{
            "The total signal strength at the checkpoints is " + std::to_string(signalStrength),
            "Read for yourself, meatbag: \n" + os.str()
        };
    }
}