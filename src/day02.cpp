#include <iostream>
#include <fstream>

#include "./days.hpp"

namespace day2 {
    namespace {
        int scoreGame(int they, int me) {
            int baseScore{ me - 'W' };
            if(baseScore == (they - 'A' + 1)) {
                return baseScore + 3;
            }

            if(me == 'X') {
                if(they == 'B') {
                    return baseScore;
                } else {
                    return baseScore + 6;
                }
            } else if(me == 'Y') {
                if(they == 'A') {
                    return baseScore + 6;
                } else {
                    return baseScore;
                }
            } else {
                if(they == 'A') {
                    return baseScore;
                } else {
                    return baseScore + 6;
                }
            }
            return 0;
        }

        int scoreGameCorrectyl(char they, char me) {
            if(me == 'Y') {
                return scoreGame(they, they + ('X' - 'A'));
            }

            if(me == 'X') {
                return scoreGame(they, they == 'A' ? 'Z' : (they == 'B' ? 'X' : 'Y'));
            }

            return scoreGame(they, they == 'A' ? 'Y' : (they == 'B' ? 'Z' : 'X'));
        }
    }

    DayResults run(std::string filename) {
        std::ifstream f{ filename };

        int totalScore{0};
        int correctScore{0};
        char meMove, theyMove;

        while (!f.eof()) {
            meMove = 0;
            theyMove = 0;
            f >> theyMove >> meMove;
            if(meMove != 0) {
                totalScore += scoreGame(theyMove, meMove);
                correctScore += scoreGameCorrectyl(theyMove, meMove);
            }
        }

        return DayResults{
            "By misunderstanding the strategy, we score " + std::to_string(totalScore) + " points.",
            "Oh wait, it should actually be " + std::to_string(correctScore) + " points."
        };
    }
}