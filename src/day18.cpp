#include <iostream>
#include <fstream>
#include <deque>
#include <tuple>
#include <sstream>
#include <string>
#include <set>
#include <vector>

#include "./days.hpp"
#include "./util.hpp"

namespace day18 {
    using std::deque;
    using std::string;
    using std::set;
    using std::vector;

    namespace {
        class Voxel {
            private:
                vector<int> m_pos;

            public:
                Voxel() {
                    m_pos = {0, 0, 0};
                }
                Voxel(int x, int y, int z) : m_pos{{x, y, z}} {}
                Voxel(string line) {
                    std::istringstream ss{line};
                    char _;
                    int x, y, z;
                    ss >> x >> _ >> y >> _ >> z;
                    m_pos = {x, y, z};
                }

                vector<Voxel> getNeighbours() {
                    vector<int> offsets{-1, 1};
                    vector<Voxel> out;
                    for(int offset : offsets) {
                        out.push_back(Voxel{m_pos[0] + offset, m_pos[1], m_pos[2]});
                        out.push_back(Voxel{m_pos[0], m_pos[1] + offset, m_pos[2]});
                        out.push_back(Voxel{m_pos[0], m_pos[1], m_pos[2] + offset});
                    }

                    return out;
                }

                vector<int> getPosition() {
                    return m_pos;
                }


                friend bool operator<(const Voxel& a, const Voxel& b);
                friend bool operator==(const Voxel& a, const Voxel& b);
                friend std::ostream& operator<<(std::ostream& os, const Voxel& v);
        };

        bool operator<(const Voxel& a, const Voxel& b) {
            return a.m_pos < b.m_pos;
        }

        bool operator==(const Voxel& a, const Voxel& b) {
            return a.m_pos == b.m_pos;
        }

        std::ostream& operator<<(std::ostream& os, const Voxel& v) {
            os << "Voxel(" << v.m_pos[0] << ", " << v.m_pos[1] << ", " << v.m_pos[2] << ")";
            return os;
        }

        struct BoundingBox {
            int minX{10000000};
            int maxX{0};
            int minY{10000000};
            int maxY{0};
            int minZ{10000000};
            int maxZ{0};
        };

        class Voloom {
            private:
                set<Voxel> m_voxels;

            public:
                Voloom(vector<Voxel> v) {
                    m_voxels.insert(v.begin(), v.end());
                }

                BoundingBox getBoundingBox() {
                    BoundingBox b;
                    for(Voxel v : m_voxels) {
                        auto p{v.getPosition()};
                        if(p[0] < b.minX) {
                            b.minX = p[0];
                        } else if(p[0] > b.maxX) {
                            b.maxX = p[0];
                        }

                        if(p[1] < b.minY) {
                            b.minY = p[1];
                        } else if(p[1] > b.maxY) {
                            b.maxY = p[1];
                        }

                        if(p[2] < b.minZ) {
                            b.minZ = p[2];
                        } else if(p[2] > b.maxZ) {
                            b.maxZ = p[2];
                        }      
                    }
                    return b;
                }

                int getTotalSurfaceArea() {
                    int surface{0};
                    for(Voxel v : m_voxels) {
                        for(Voxel n : v.getNeighbours()) {
                            if(m_voxels.find(n) == m_voxels.end()) {
                                ++surface;
                            }
                        }
                    }

                    return surface;
                }

                int getOuterSurfaceArea() {
                    int surface{0};

                    BoundingBox bb{getBoundingBox()};
                    bb.minX--;
                    bb.maxX++;
                    bb.minY--;
                    bb.maxY++;
                    bb.minZ--;
                    bb.maxZ++;

                    deque<Voxel> edge;
                    set<Voxel> done;
                    edge.push_back(Voxel{bb.minX, bb.minY, bb.minZ});

                    while(!edge.empty()) {
                        Voxel at{edge.front()};
                        edge.pop_front();
                        for(Voxel n : at.getNeighbours()) {
                            if(!done.contains(n)) {
                                if(m_voxels.contains(n)) {
                                    ++surface;
                                } else {
                                    vector<int> np = n.getPosition();
                                    if(np[0] >= bb.minX && np[0] <= bb.maxX &&
                                        np[1] >= bb.minY && np[1] <= bb.maxY &&
                                        np[2] >= bb.minZ && np[2] <= bb.maxZ &&
                                        std::find(edge.begin(), edge.end(), n) == edge.end()) {
                                            edge.push_back(n);
                                        }
                                }
                            }
                        }
                        done.insert(at);
                    }

                    return surface;
                }
        };
    }

    DayResults run(string filename) {
        std::ifstream f{filename};

        vector<Voxel> voxels;

        while(!f.eof()) {
            string line;
            std::getline(f, line);
            if(line != "") {
                voxels.push_back(Voxel{line});
            }
        }

        Voloom vol{voxels};
        int part1{vol.getTotalSurfaceArea()};
        int part2{vol.getOuterSurfaceArea()};

        return DayResults{
            std::to_string(part1),
            std::to_string(part2)
        };
    }
}