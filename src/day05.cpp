#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <string>
#include <deque>
#include <vector>

#include "./days.hpp"

namespace day5 {
    using std::string;
    using std::vector;

    using Stack = std::deque<char>;
    using Stacks = std::vector<Stack>;

    namespace {
        struct Instruction {
            int nCrates{};
            int fromIndex{};
            int toIndex{};
        };

        auto parseStacks(std::ifstream& f) {
            Stacks stax{};
            string line{};

            while(true) {
                std::getline(f, line);

                if(line[1] == '1') {
                    break;
                }

                std::istringstream ss{line};
                int stack{0};
                while(!ss.eof()) {
                    if(stax.size() <= stack) {
                        stax.push_back({});
                    }

                    char _, label;
                    ss >> std::noskipws >> _ >> label >> _;
                    ss >> std::noskipws >> _;
                    if(label != ' ') {
                        stax[stack].push_front(label);
                    }
                    stack += 1;
                }            
            }

            return stax;
        }

        vector<Instruction> parseInstructions(std::ifstream& f) {
            string line{};
            string trash{};
            int n{0};
            int from{0};
            int to{0};
            vector<Instruction> instructions{};

            while(!f.eof()) {
                std::getline(f, line);
                if(line.size() > 0) {
                    std::istringstream iss{line};
                    
                    // move <n> from <from + 1> to <to + 1>
                    iss >> trash >> n >> trash >> from >> trash >> to;

                    instructions.push_back({n, from - 1, to - 1});
                }
            }

            return instructions;
        }

        Stacks runInstructions9000(vector<Instruction> instructions, Stacks stax) {
            for(Instruction isnt : instructions) {
                auto fromIt = stax[isnt.fromIndex].end();
                std::reverse_copy(fromIt - isnt.nCrates, fromIt, std::back_inserter(stax[isnt.toIndex]));

                int ogSize = stax[isnt.fromIndex].size();
                stax[isnt.fromIndex].resize(ogSize - isnt.nCrates);
            }

            return stax;
        }

        Stacks runInstructions9001(vector<Instruction> instructions, Stacks stax) {
            for(Instruction isnt : instructions) {
                auto fromIt = stax[isnt.fromIndex].end();
                std::copy(fromIt - isnt.nCrates, fromIt, std::back_inserter(stax[isnt.toIndex]));
                
                int ogSize = stax[isnt.fromIndex].size();
                stax[isnt.fromIndex].resize(ogSize - isnt.nCrates);
            }

            return stax;
        }

        string mekAnswer(Stacks stax) {
            std::ostringstream st{};
            for(Stack s : stax) {
                st << s.back();
            }
            return st.str(); 
        }
    }

    DayResults run(string filename) {
        std::ifstream f{filename};

        Stacks stax = parseStacks(f);
        
        // Flush the empty line
        string empty;
        std::getline(f, empty);

        vector<Instruction> instructions = parseInstructions(f);

        const Stacks part1 = runInstructions9000(instructions, stax);

        const Stacks part2 = runInstructions9001(instructions, stax);

        return DayResults {
            "If we HAD a 9000, the anwer would be " + mekAnswer(part1),
            "However, our shiny new 9001 actually produces " + mekAnswer(part2)
        };
    }
}