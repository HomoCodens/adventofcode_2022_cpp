#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "./days.hpp"
#include "./util.hpp"
#include "./vector2d.hpp"

namespace day15 {
    using std::string;
    using std::vector;
    using std::unordered_set;
    using PointSet = unordered_set<Vector2D, PointHasher>;

    namespace {
        class Sensor {
            private:
                Vector2D m_position;
                Vector2D m_closestBeacon;

            public:
                Sensor(const Vector2D& pos, const Vector2D& beaconPos) : m_position{pos}, m_closestBeacon{beaconPos} {}
                Sensor(string line) {
                    // Sensor at x=<int>, y=<int>: closest beacon is at x=<int>, y=<int>
                    string _;
                    char _c;
                    int posX, posY, beaconX, beaconY;
                    std::istringstream ss{line};

                    ss.ignore(12);
                    ss >> posX;
                    ss.ignore(4);
                    ss >> posY;
                    ss.ignore(25);
                    ss >> beaconX;
                    ss.ignore(4);
                    ss >> beaconY;

                    m_position = Vector2D{posX, posY};
                    m_closestBeacon = Vector2D{beaconX, beaconY};
                }

                Vector2D getPosition() {
                    return m_position;
                }

                Vector2D getBeacon() {
                    return m_closestBeacon;
                }

                PointSet whatSeeYouAt(int y) {
                    PointSet s;
                    int distToBeacon = manhattanDist(m_position, m_closestBeacon);
                    int distToRow = std::abs(m_position.y - y);
                    if(distToBeacon < distToRow) {
                        return s;
                    }

                    for(int dx = -(distToBeacon - distToRow); dx <= distToBeacon - distToRow; ++dx) {
                        s.insert(Vector2D{m_position.x + dx, y});
                    }

                    return s;
                }

                bool isThereAThingAt(Vector2D& p) {
                    return m_position == p || m_closestBeacon == p;
                }

                friend std::ostream& operator<<(std::ostream& os, const Sensor& s);
        };

        std::ostream& operator<<(std::ostream& os, const Sensor& s) {
            os << "A Sensor at " << s.m_position << '\n';
            os << "seeing a Beacon at " << s.m_closestBeacon << '\n';

            return os;
        }
    }

    DayResults run(string filename) {
        int rowInQuestion;

        if(filename.substr(filename.find_last_of("/") + 1) == "test.txt") {
            rowInQuestion = 10;
        } else {
            rowInQuestion = 2000000;
        }

        std::ifstream f{filename};
        string line;
        vector<Sensor> sensors;
        while(!f.eof()) {
            line = "";
            std::getline(f, line);

            if(line != "") {
                sensors.push_back(Sensor{line});
            }
        }

        PointSet allThePoints;
        for(Sensor s : sensors) {
            std::cout << s << '\n';
            PointSet ps{s.whatSeeYouAt(rowInQuestion)};
            allThePoints.insert(ps.begin(), ps.end());
        }

        for(Sensor s : sensors) {
            allThePoints.erase(s.getPosition());
            allThePoints.erase(s.getBeacon());
        }

        /*for(Vector2D p : allThePoints) {
            std::cout << p << '\n';
        }*/

        /*Sensor b{"Sensor at x=0, y=0: closest beacon is at x=2, y=-1"};
        Sensor c{"Sensor at x=3, y=0: closest beacon is at x=2, y=0"};

        std::cout << b;
        PointSet ps{b.whatSeeYouAt(0)};

        std::cout << manhattanDist(b.getPosition(), b.getBeacon()) << '\n';

        PointSet ps2{c.whatSeeYouAt(0)};
        ps.insert(ps2.begin(), ps2.end());

        for(Vector2D bla : ps) {
            std::cout << bla << '\n';
        }*/

        return DayResults{
            std::to_string(allThePoints.size())
        };
    }
}