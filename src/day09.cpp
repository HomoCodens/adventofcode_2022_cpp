#include <fstream>
#include <string>
#include <unordered_map>

#include "./days.hpp"
#include "./util.hpp"

namespace day9 {
    using std::string;

    namespace {
        struct Vector2D {
            int x{0};
            int y{0};
        };

        Vector2D operator+(Vector2D a, Vector2D b) {
            return Vector2D{a.x + b.x, a.y + b.y};
        }

        Vector2D operator-(Vector2D a, Vector2D b) {
            return Vector2D{a.x - b.x, a.y - b.y};
        }

        Vector2D operator*(int magnitude, Vector2D v) {
            return Vector2D{magnitude*v.x, magnitude*v.y};
        }

        // Boi, really leaning into custom types today
        struct Move {
            char dir{'U'};
            int dist{0};
        };

        class DopeyTheRopey {
            private:
                Vector2D m_head{0, 0};
                Vector2D m_tail {0, 0};
                std::unordered_map<string, bool> m_nodesVisitedByTail {{"(0, 0)", true}};

                Vector2D getVelocityVector(char dir) {
                    switch(dir) {
                        case 'U':
                            return Vector2D{0, 1}; // And thus, god defined the directions of the world
                        case 'R':
                            return Vector2D{1, 0};
                        case 'D':
                            return Vector2D{0, -1};
                        case 'L':
                            return Vector2D{-1, 0};
                    }

                    return Vector2D{0, 0};
                }

                void catchUpTail() {
                    Vector2D awayness = m_head - m_tail;
                    if(std::abs(awayness.x) > 1 || std::abs(awayness.y) > 1) {
                        Vector2D unitAwayness = Vector2D{
                            awayness.x == 0 ? 0 : awayness.x / std::abs(awayness.x),
                            awayness.y == 0 ? 0 : awayness.y / std::abs(awayness.y)
                        };

                        m_tail = m_tail + unitAwayness;

                        string kee = "(" + std::to_string(m_tail.x) + ", " + std::to_string(m_tail.y) + ")";
                        m_nodesVisitedByTail.insert_or_assign(kee, true);
                    }
                }

            public:
                void move(Move m) {
                    Vector2D v = getVelocityVector(m.dir);

                    for(int i = 0; i < m.dist; i++) {
                        m_head = m_head + v;
                        catchUpTail();
                    }
                }

                int ohWhereHasTheTailBeen() {
                    return m_nodesVisitedByTail.size();
                }
        };
    }

    DayResults run(string filename) {
        std::ifstream f(filename);

        DopeyTheRopey rope{};

        while(!f.eof()) {
            char dir{'\0'};
            int dist{0};
            f >> dir >> dist;
            if(dir != '\0') {
                rope.move(Move{dir, dist});
            }
        }

        return DayResults{
            std::to_string(rope.ohWhereHasTheTailBeen())
        };
    }
}