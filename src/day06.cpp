#include <fstream>
#include <string>

#include "./days.hpp"
#include "./util.hpp"

namespace day6 {
    using std::string;

    namespace {
        int detect(string& ds, int lengthOfMarker) {
            auto length{ds.size()};

            for(int start = 0; start < (length - lengthOfMarker); ++start) {
                bool valid = true;
                for(int i = 0; i < lengthOfMarker - 1; ++i) {
                    for(int j = i + 1; j < lengthOfMarker; ++j) {
                        if(ds[start + i] == ds[start + j]) {
                            valid = false;
                            break;
                        }
                    }
                    if(!valid) {
                        break;
                    }
                }
                if(valid) {
                    return start + lengthOfMarker;
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