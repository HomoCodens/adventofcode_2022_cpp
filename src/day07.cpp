#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "./days.hpp"
#include "./util.hpp"

namespace day7 {
    using std::string;
    using std::vector;

    namespace {

        // Why reinvent trees if we already have a perfectly good iterator, eh?
        long dig(std::ifstream& f, long& runningTotal, vector<long>& sizes) {
            string line;
            long dirSize{0};

            while(!f.eof()) {
                // munchmunchmunch
                std::getline(f, line);
                
                // We have a command or have reached the end of the file (implicit "$ cd ..")
                if(line == "" || line[0] == '$') {

                    // We are done with this dir
                    if(line == "" || line == "$ cd ..") {

                        // For part 1
                        if(dirSize <= 100000) {
                            runningTotal += dirSize;
                        }

                        // For part 2
                        sizes.push_back(dirSize);
                        return dirSize;
                   
                    // We need to go deeper (said noone since 2011)
                    } else if(line[2] == 'c') {
                        dirSize += dig(f, runningTotal, sizes);
                    }

                // The line is a listing item, ignore dirs
                } else if(line[0] != 'd') {
                    std::istringstream is{line};
                    int size{0};
                    is >> size;
                    dirSize += size;
                }
            }

            return dirSize;
        }
    }

    DayResults run(string filename) {
        std::ifstream f{filename};

        long part1{0};
        vector<long> allTheSizesOfAllTheDirs{};
        long totalSize{dig(f, part1, allTheSizesOfAllTheDirs)};

        long diskSize{70000000L};
        long requiredSpace{30000000L};

        long availableSpace{diskSize - totalSize};
        long needToFree{requiredSpace - availableSpace};

        long part2{diskSize};
        for(long s : allTheSizesOfAllTheDirs) {
            if(s >= needToFree && s < part2) {
                part2 = s;
            }
        }

        return DayResults{
            "The smaller dirs sum up to " + std::to_string(part1),
            "The biglyest smallest dir we can delete is " + std::to_string(part2) + " elfbytes in size."
        };
    }
}