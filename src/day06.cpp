#include <fstream>
#include <string>
#include <vector>

#include "./days.hpp"
#include "./util.hpp"

namespace day6 {
    using std::string;
    namespace {
        int detect(string& ds, int lengthOfMarker) {
            std::vector<int> charCounts(26, 0);
            
            auto length{ds.size()};
            int badity{0};

            for(int ptr = 0; ptr < length; ++ptr) {
                int i = ds[ptr] - 'a';

                // "Rising edge" -> one more char is now contributing to
                // the current window being bad
                badity += (charCounts[i] == 1);
                charCounts[i] += 1;

                if(ptr >= lengthOfMarker) {
                    int j = ds[ptr - lengthOfMarker] - 'a';
                    
                    // "Falling edge"
                    badity -= (charCounts[j] == 2);
                    charCounts[j] -= 1;
                }

                if(ptr >= (lengthOfMarker - 1) && badity == 0) {
                    return ptr + 1;
                }
            }
            return 0;
        }
    }

    DayResults run(string filename) {
        std::ifstream f{filename};
        string datastream;
        std::getline(f, datastream);

        int sop = detect(datastream, 4);
        int som = detect(datastream, 14);

        return DayResults {
            std::to_string(sop),
            std::to_string(som)
        };
    }
}