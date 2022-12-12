#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "./days.hpp"
#include "./util.hpp"
#include "./vector2d.hpp"

namespace day9 {
    using std::string;

    namespace {

        // Boi, really leaning into custom types today
        struct Move {
            char dir{'U'};
            int dist{0};
        };

        class DopeyTheRopey {
            private:
                std::vector<Vector2D> m_knots{};
                std::unordered_map<Vector2D, bool, PointHasher> m_nodesVisitedByTail {{Vector2D{}, true}};

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

                bool catchUpKnot(int knot) {
                    Vector2D leader{m_knots[knot - 1]};
                    Vector2D follower{m_knots[knot]};
                    Vector2D awayness = leader - follower;

                    if(std::abs(awayness.x) > 1 || std::abs(awayness.y) > 1) {
                        Vector2D unitAwayness = Vector2D{
                            awayness.x == 0 ? 0 : awayness.x / std::abs(awayness.x),
                            awayness.y == 0 ? 0 : awayness.y / std::abs(awayness.y)
                        };

                        m_knots[knot] = m_knots[knot] + unitAwayness;

                        return true;
                    } else {
                        return false;
                    }
                }

            public:
                DopeyTheRopey(int length) {
                    for(int i = 0; i < length; ++i) {
                        m_knots.push_back(Vector2D{0, 0});
                    }
                }

                void move(Move m) {
                    Vector2D v = getVelocityVector(m.dir);

                    for(int i = 0; i < m.dist; i++) {
                        m_knots[0] = m_knots[0] + v;
                        int k = 1;
                        while(k < m_knots.size() && catchUpKnot(k)) {
                            ++k;
                        }

                        m_nodesVisitedByTail.insert_or_assign(m_knots[m_knots.size() - 1], true);
                    }
                }

                int ohWhereHasTheTailBeen() {
                    return m_nodesVisitedByTail.size();
                }
        };
    }

    DayResults run(string filename) {
        std::ifstream f(filename);

        DopeyTheRopey rope{2};
        DopeyTheRopey longRope{10};

        while(!f.eof()) {
            char dir{'\0'};
            int dist{0};
            f >> dir >> dist;
            if(dir != '\0') {
                Move m{dir, dist};
                rope.move(m);
                longRope.move(m);
            }
        }

        return DayResults{
            "The short rope tail goes " + std::to_string(rope.ohWhereHasTheTailBeen()) + " many places.",
            "The long rope tail goes " + std::to_string(longRope.ohWhereHasTheTailBeen()) + " many places."
        };
    }
}