#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <string>
#include <deque>
#include <vector>

namespace day5 {
    using std::string;
    using std::vector;

    using Stack = std::deque<char>;
    using Stacks = std::vector<Stack>;

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
        std::regex number_regex{"\\d+"};
        vector<Instruction> instructions{};
        while(!f.eof()) {
            std::getline(f, line);
            if(line.size() > 0) {
                auto it{std::regex_iterator(line.begin(), line.end(), number_regex)};
                
                // This feels like it should not be it..
                int n{std::atoi((*(it++)).str().c_str())};
                int from{std::atoi((*(it++)).str().c_str()) - 1};
                int to{std::atoi((*(it++)).str().c_str()) - 1};

                instructions.push_back({n, from, to});
            }
        }

        return instructions;
    }

    Stacks runInstructions9000(vector<Instruction> instructions, Stacks stax) {
        for(Instruction isnt : instructions) {
            for(int i = 0; i < isnt.nCrates; i++) {
                stax[isnt.toIndex].push_back(stax[isnt.fromIndex].back());
                stax[isnt.fromIndex].pop_back();
            }
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

    void printResult(Stacks stax) {
        for(Stack s : stax) {
            std::cout << s.back();
        }
        std::cout << '\n';
    }

    void run(string filename) {
        std::ifstream f{filename};

        Stacks stax = parseStacks(f);
        
        // Flush the empty line
        string empty;
        std::getline(f, empty);

        vector<Instruction> instructions = parseInstructions(f);

        const Stacks part1 = runInstructions9000(instructions, stax);
        printResult(part1);

        const Stacks part2 = runInstructions9001(instructions, stax);
        printResult(part2);
    }
}