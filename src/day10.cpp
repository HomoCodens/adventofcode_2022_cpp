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
        string line;
        while(!f.eof() && (cycle <= 222)) {
            instruction = "";
            y = 0;
            std::getline(f, line);

            // TODO: Why does f >> instruction >> y; crap out after the first noop??
            std::istringstream is{line};
            is >> instruction >> y;
            std::cout << "Instraction be " << instruction << ' ' << y << '\n';
            
            if(line != "") {
                if(instruction == "noop") {
                    ++cycle;
                    if((cycle - 20) % 40 == 0) {
                        signalStrength += cycle*x;
                    }
                } else {
                    cycle += 2;
                    x += y;
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

        return DayResults{
            std::to_string(signalStrength)
        };
    }
}