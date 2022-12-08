#include <iostream>
#include <fstream>
#include <string>

#include "./util.hpp"

namespace day8 {
    using std::string;
    using ForestIndex = int;

    namespace {
        struct View {
            int length{0};
            bool edgeVisible{false};
        };

        View look(string forest, ForestIndex from, int direction, int nRow, int nCol) {
            int lengthOfView{1};    // Can always see at least one tree
            char tree{forest[from]};
            ForestIndex at{from + direction};
            bool isAnEdge{false};
            while(!isAnEdge && forest[at] < tree) {
                isAnEdge = (at % nCol) == 0 ||              // left edge
                            (at % nCol) == (nCol - 1) ||    // right edge
                            at < nCol ||                    // top edge
                            at >= (nRow - 1)*nCol;          // bottom edge

                if(!isAnEdge) {
                    ++lengthOfView;
                    at += direction;
                }
            }

            return View{
                lengthOfView,
                isAnEdge && forest[at] < tree
            };
        }
    }

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

        int visibleTrees{2*nRow + 2*nCol - 4};

        // Trees on the edge will always score 0
        int scenicScore{0};

        // up, right, down, left
        std::vector<int> directions{-nCol, 1, nCol, -1};

        for(int r = 1; r < (nRow - 1); ++r) {
            for(int c = 1; c < (nCol - 1); ++c) {
                bool visible{false};
                int score{1};
                for(int d : directions) {
                    View v = look(forest, r*nCol + c, d, nRow, nCol);
                    visible |= v.edgeVisible;
                    score *= v.length;
                }
                visibleTrees += visible;
                scenicScore = scenicScore < score ? score : scenicScore;
            }
        }

        return DayResults{
            "There are " + std::to_string(visibleTrees) + " trees visible from the outside.",
            std::to_string(scenicScore)
        };
    }
}