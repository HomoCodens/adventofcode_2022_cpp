#include <iostream>
#include <fstream>
#include <limits>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>

#include "./days.hpp"
#include "./util.hpp"
#include "./vector2d.hpp"

namespace day15 {
    using std::string;
    using std::vector;
    using std::map;
    using PointSet = std::unordered_set<Vector2D, PointHasher>;

    namespace {
        // An interval [m_min, m_max)
        class InterWhale {
            private:
                int m_min;
                int m_max;

            public:
                InterWhale() : m_min{std::numeric_limits<int>::min()}, m_max{std::numeric_limits<int>::max()} {}
                InterWhale(int min, int max) : m_min{min}, m_max{max} {}

                bool intersects(const InterWhale& other) {
                    return (m_min <= other.m_min && (m_max - 1) >= other.m_min) ||
                            (other.m_min <= m_min && (other.m_max - 1) >= m_min);
                }

                bool touches(const InterWhale& other) {
                    return m_max == other.m_min || other.m_max == m_min;
                }

                InterWhale vnion(const InterWhale& other) {
                    if(!this->intersects(other) && !this->touches(other)) {
                        throw "Bad boi!";
                    }
                    return InterWhale{std::min(m_min, other.m_min), std::max(m_max, other.m_max)};
                }

                InterWhale int3rs3ction(const InterWhale& other) {
                    if(!this->intersects(other)) {
                        return InterWhale{0,0};
                    }

                    return InterWhale{
                        std::max(m_min, other.m_min),
                        std::min(m_max, other.m_max)
                    };
                }

                vector<int> getAllPoints() {
                    vector<int> out;
                    // 'msure there is some cleverer way
                    for(int i = m_min; i < m_max; ++i) {
                        out.push_back(i);
                    }
                    return out;
                }

                // yeh yeh, size_t
                int size() {
                    return m_max - m_min - 1;
                }

                friend class UberWhale;
                friend std::ostream& operator<<(std::ostream& os, InterWhale iw);
        };

        std::ostream& operator<<(std::ostream& os, InterWhale iw) {
            os << '[' << iw.m_min << ',' << iw.m_max << ')';
            return os;
        }

        class UberWhale {
            private:
                // Map of lower bound -> interval so we can keep them sorted
                map<int, InterWhale> m_childs;

            public:
                void add(InterWhale iw) {
                    if(iw.size() == 0) {
                        return;
                    }

                    if(m_childs.size() == 0) {
                        m_childs.insert_or_assign(iw.m_min, iw);
                        return;
                    }

                    // As in "this is it" ;P
                    auto it{m_childs.upper_bound(iw.m_min)};

                    // We fall outside the range of present intervals (also handles the case m_childs.size() == 1)
                    if(it == m_childs.begin() || it == m_childs.end()) {
                        if(it == m_childs.end()) {
                            --it;
                        }
                        if(it->second.intersects(iw) || it->second.touches(iw)) {
                            iw = iw.vnion(it->second);
                            m_childs.erase(it);
                        }
                        m_childs.insert_or_assign(iw.m_min, iw);
                        return;
                    }

                    // Look at the lower one first (helps with the thinking :shrug:)
                    --it;
                    if(it->second.intersects(iw) || it->second.touches(iw)) {
                        iw = iw.vnion(it->second);
                        it = m_childs.erase(it); // Now points at the element after the one removed -> the original it
                    } else {
                        it++;
                    }

                    // coulda done a loop but again, the thinks
                    if(it->second.intersects(iw) || it->second.touches(iw)) {
                        iw = iw.vnion(it->second);
                        m_childs.erase(it);
                    }

                    m_childs.insert_or_assign(iw.m_min, iw);
                }

                UberWhale int3rs3ction(const InterWhale& other) {
                    UberWhale out;
                    for(auto bla : m_childs) {
                        if(bla.second.intersects(other)) {
                            out.add(bla.second.int3rs3ction(other));
                        }
                    }

                    return out;
                }

                int size() {
                    int s{0};
                    for(auto choochoo : m_childs) {
                        s += choochoo.second.size();
                    }

                    return s;
                }

                bool hazHoles() {
                    return m_childs.size() > 1;
                }

                vector<int> holezPositions() {
                    vector<int> holz{};
                    
                    if(!hazHoles()) {
                        return holz;
                    }

                    // can you tell it is late? ;P
                    auto vroom{m_childs.begin()};

                    for(auto vroom : m_childs) {
                        holz.push_back(vroom.second.m_max);
                    }
                }

                InterWhale boundingInterWhale() {
                    // we'll think about it later...
                    return m_childs.begin()->second;
                }

                vector<int> getAllPoints() {
                    vector<int> out;
                    for(auto i : m_childs) {
                        auto pnts{i.second.getAllPoints()};
                        out.insert(out.end(), pnts.begin(), pnts.end());
                    }

                    return out;
                }

                friend std::ostream& operator<<(std::ostream& os, UberWhale uw);
        };

        std::ostream& operator<<(std::ostream& os, UberWhale uw) {
            for(auto i : uw.m_childs) {
                std::cout << i.second << ' ';
            }
            return os;
        }
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

                InterWhale whatSeeYouAt(int y) {
                    int distToBeacon = manhattanDist(m_position, m_closestBeacon);
                    int distToRow = std::abs(m_position.y - y);
                    if(distToBeacon < distToRow) {
                        return InterWhale{0, 0};
                    }

                    int offset{distToBeacon - distToRow};
                    return InterWhale{m_position.x - offset, m_position.x + offset + 1};
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
        int searchLimit;

        if(filename.substr(filename.find_last_of("/") + 1) == "test.txt") {
            rowInQuestion = 10;
            searchLimit = 20;
        } else {
            rowInQuestion = 2000000;
            searchLimit = 4000000;
        }
        
        /*InterWhale iw1{0, 10};
        InterWhale iw2{5, 12};
        InterWhale iw3{16, 20};

        UberWhale uw;
        uw.add(iw1);
        uw.add(iw2);
        uw.add(iw3);
        std::cout << uw << '\n';
        uw.add(InterWhale{-4, -3});
        std::cout << uw << '\n';
        uw.add(InterWhale{14, 16});
        std::cout << uw << '\n';
        uw.add(InterWhale{2, 18});
        std::cout << uw << '\n';

        for(int blub : uw.getAllPoints()) {
            std::cout << blub << '\n';
        }*/

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

        UberWhale allTheWhales;
        for(Sensor s : sensors) {
            InterWhale pnts{s.whatSeeYouAt(rowInQuestion)};
            allTheWhales.add(pnts);
        }

        std::cout << allTheWhales << '\n';

        PointSet allThePoints;
        vector<int> ap{allTheWhales.getAllPoints()};
        for(int xPos : ap) {
            allThePoints.insert(Vector2D{xPos, rowInQuestion});
        }

        for(Sensor s : sensors) {
            allThePoints.erase(s.getPosition());
            allThePoints.erase(s.getBeacon());
        }

        InterWhale searchInterWhale{0, searchLimit + 1};
        for(int riq = 0; riq < searchLimit; ++riq) {
            UberWhale rowContents;
            for(Sensor s : sensors) {
                rowContents.add(s.whatSeeYouAt(riq));
            }

            UberWhale lookHere{rowContents.int3rs3ction(searchInterWhale)};

            if(lookHere.size() != searchLimit) {
                std::cout << "row no. " << riq << '\n';
                std::cout << "lookHere size: " << lookHere.size() << '\n';
                std::cout << lookHere << '\n';
                std::cout << lookHere.hazHoles() << '\n';
                std::cout << '\n';
                break;
            }
        }

        std::cout << (4000000L * 3120101 + 2634249L) << '\n';

        return DayResults{
            std::to_string(allThePoints.size())
        };
    }
}