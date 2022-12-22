#include <iostream>
#include <fstream>
#include <tuple>
#include <sstream>
#include <string>
#include <set>
#include <vector>

#include "./days.hpp"
#include "./util.hpp"

namespace day18 {
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

                friend std::ostream& operator<<(std::ostream& os, const Voxel& v);
        };

        std::ostream& operator<<(std::ostream& os, const Voxel& v) {
            os << "Voxel(" << v.m_pos[0] << ", " << v.m_pos[1] << ", " << v.m_pos[2] << ")";
            return os;
        }
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

        for(Voxel v : voxels) {
            std::cout << v << '\n';
        }

        Voxel o{};
        for(Voxel n : o.getNeighbours()) {
            std::cout << n << '\n';
        }

        set<vector<int>> filled;
        for(Voxel v : voxels) {
            filled.insert(v.getPosition());
        }

        int part1{0};
        for(Voxel v : voxels) {
            for(Voxel n : v.getNeighbours()) {
                if(filled.find(n.getPosition()) == filled.end()) {
                    part1++;
                }
            }
        }

        return DayResults{
            std::to_string(part1)
        };
    }
}