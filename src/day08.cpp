#include <fstream>
#include <string>

#include "./util.hpp"

namespace day8 {
    using std::string;

    DayResults run(string filename) {
        std::ifstream f{filename};

        string forest{""};
        int nRow{0};
        
        while(!f.eof()){
            string line{""};
            f >> line;
            if(line != "") {
                forest += line;
                ++nRow;
            }
        }

        int nCol{forest.size() / nRow};

        std::vector<int> visabilityMap(nRow*nCol, 0);

        // Horizontal
        for(int r = 1; r < (nRow - 1); ++r) {
            int ri{r*nCol};
            char leftRay{0};
            char rightRay{0};

            for(int c = 1; c < (nCol - 1); ++c) {
                int outerLeft = ri + c - 1;
                int innerLeft = ri + c;
                int innerRight = ri + nCol - c - 1;
                int outerRight = ri + nCol - c;
                leftRay = forest[outerLeft] > leftRay ? forest[outerLeft] : leftRay;
                rightRay = forest[outerRight] > rightRay ? forest[outerRight] : rightRay;

                visabilityMap[innerLeft] += (forest[innerLeft] > leftRay);
                visabilityMap[innerRight] += (forest[innerRight] > rightRay);
            }
        }

        // Vertical
        for(int c = 1; c < (nCol - 1); ++c) {
            char topRay{0};
            char bottomRay{0};

            for(int r = 1; r < (nRow - 1); ++r) {
                int outerTop = (r - 1)*nCol + c;
                int innerTop = r*nCol + c;
                int innerBottom = (nRow - r - 1)*nCol + c;
                int outerBottom = (nRow - r)*nCol + c;

                topRay = forest[outerTop] > topRay ? forest[outerTop] : topRay;
                bottomRay = forest[outerBottom] > bottomRay ? forest[outerBottom] : bottomRay;

                visabilityMap[innerTop] += (forest[innerTop] > topRay);
                visabilityMap[innerBottom] += (forest[innerBottom] > bottomRay);
            }
        }

        int visibleTrees{2*nRow + 2*nCol - 4};
        for(int tree : visabilityMap) {
            visibleTrees += (tree > 0);
        }

        return DayResults{
            "There are " + std::to_string(visibleTrees) + " trees visible from the outside."
        };
    }
}