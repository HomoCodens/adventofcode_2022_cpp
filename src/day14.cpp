#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "./days.hpp"
#include "./util.hpp"
#include "./vector2d.hpp"

namespace day14 {
    using std::string;
    using std::vector;

    namespace {
        struct Corner {
            Vector2D min{0, 0};
            Vector2D max{0, 0};
        };

        class Path {
            private:
                vector<Vector2D> m_points{};

            public:
                Path(string s) {
                    std::istringstream ss{s};
                    int col;
                    int row;
                    char comma;
                    string arrow;
                    while(!ss.eof()) {
                        col = -1;
                        ss >> col >> comma >> row;
                        if(col >= 0) {
                            m_points.push_back(Vector2D{col, row});
                            ss >> arrow;
                        }
                    }
                }

                vector<Vector2D> getPoints() {
                    vector<Vector2D> pnts{};

                    if(m_points.size() == 0) {
                        return pnts;
                    }
                    pnts.push_back(m_points[0]);
                    for(int i = 1; i < m_points.size(); ++i) {
                        Vector2D at{m_points[i - 1]};
                        Vector2D end{m_points[i]};
                        Vector2D dir{end - at};
                        if(dir.x == 0) {
                            dir.y = dir.y / std::abs(dir.y);
                        } else {
                            dir.x = dir.x / std::abs(dir.x);
                        }
                        while((at = at + dir) != end) {
                            pnts.push_back(at);
                        }
                        pnts.push_back(end);
                    }

                    return pnts;
                }

                Corner getExtrema() {
                    Corner c{
                        m_points[0],
                        m_points[0]
                    };
                    for(Vector2D v : m_points) {
                        if(v.x < c.min.x) {
                            c.min.x = v.x;
                        }
                        
                        if(v.x > c.max.x) {
                            c.max.x = v.x;
                        }

                        if(v.y < c.min.y) {
                            c.min.y = v.y;
                        }
                        
                        if(v.y > c.max.y) {
                            c.max.y = v.y;
                        }
                    }

                    return c;
                }

                friend std::ostream& operator<<(std::ostream& os, const Path& p);
        };

        class SandLand {
            private:
                vector<vector<char>> m_map{};
                int m_colOffset{0};
                int m_sands{0};

                Corner getMapCorners(vector<Path>& paths) {
                    if(paths.size() == 0) {
                        return Corner{};
                    }

                    Corner c = paths[0].getExtrema();
                    for(Path p : paths) {
                        Corner pc{p.getExtrema()};

                        if(pc.min.x < c.min.x) {
                            c.min.x = pc.min.x;
                        }
                        
                        if(pc.max.x > c.max.x) {
                            c.max.x = pc.max.x;
                        }

                        if(pc.min.y < c.min.y) {
                            c.min.y = pc.min.y;
                        }
                        
                        if(pc.max.y > c.max.y) {
                            c.max.y = pc.max.y;
                        }
                    }

                    return c;
                }

                bool dropAGrain() {
                    // std::cout << "Now dropping grain no. " << std::to_string(m_sands) << '\n';

                    Vector2D pos{500 - m_colOffset, 0};
                    while(pos.y < m_map.size() && pos.x >= 0 && pos.x < m_map[0].size()) {
                        if(pos.y == (m_map.size() - 1) || m_map[pos.y + 1][pos.x] == '.') {
                            pos = pos + Vector2D{0, 1};
                        } else if(pos.x == 0 || m_map[pos.y + 1][pos.x - 1] == '.') {
                            pos = pos + Vector2D{-1, 1};
                        } else if(pos.x == (m_map[0].size() - 1) || m_map[pos.y + 1][pos.x + 1] == '.') {
                            pos = pos + Vector2D{1, 1};
                        } else {
                            break;
                        }
                    }

                    // we're full up (part2)
                    if(pos.x == (500 - m_colOffset) && pos.y == 0) {
                        ++m_sands;
                        return false;
                    }

                    if(pos.y < m_map.size() && pos.x > 0 && pos.x < m_map[0].size()) {
                        m_map[pos.y][pos.x] = 'o';
                        ++m_sands;
                        return true;
                    } else {
                        return false;
                    }
                }

            public:
                SandLand(vector<Path> paths, bool part1 = true) {
                    Corner mapCorners{getMapCorners(paths)};

                    if(!part1) {
                        string newPath{"0," + std::to_string(mapCorners.max.y + 2) + 
                                        " -> " + "1000," + std::to_string(mapCorners.max.y + 2)};
                        paths.push_back(Path{newPath});
                        mapCorners = getMapCorners(paths);
                    }

                    m_colOffset = mapCorners.min.x;

                    vector<vector<char>> map{};
                    int mapWidth = mapCorners.max.x - mapCorners.min.x + 1;
                    for(int i = 0; i <= mapCorners.max.y; ++i) {
                        map.push_back(vector<char>(mapWidth, '.'));
                    }

                    for(Path& p : paths) {
                        for(Vector2D point : p.getPoints()) {
                            map[point.y][point.x - m_colOffset] = '#';
                        }
                    }

                    map[0][500 - m_colOffset] = '+';

                    m_map = map;
                }

                // For debugging purpoises (damn turtles...)
                void shrinkMap(int to) {
                    m_map.resize(to);
                }

                int sand() {
                    while(dropAGrain()){
                        //std::cout << *this;
                    };
                    /*for(int i = 0; i < 40; i++) {
                        dropAGrain();
                        std::cout << "Grain no. " << i << '\n' << *this << "\n\n\n";
                    }*/
                    return m_sands;
                }

                friend std::ostream& operator<<(std::ostream& os, const SandLand& sl);
        };

        std::ostream& operator<<(std::ostream& os, const Path& p) {
            for(Vector2D v : p.m_points) {
                os << v << " -> ";
            }

            return os;
        }

        std::ostream& operator<<(std::ostream& os, const Corner& c) {
            os << "Min: " << c.min << "\nMax: " << c.max << '\n';
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const SandLand& sl) {
            for(auto row : sl.m_map) {
                for(char c : row) {
                    os << c;
                }
                os << '\n';
            }

            return os;
        }
    }


    DayResults run(string filename) {
        std::ifstream f{filename};
        
        vector<Path> paths{};
        
        string line;
        while(!f.eof()) {
            line = "";
            std::getline(f, line);
            if(line != "") {
                paths.push_back(Path{line});
            }
        }

        SandLand sl{paths};
        //std::cout << sl << "\n\n";
        int part1{sl.sand()};
        //std::cout << sl << '\n';

        SandLand sl2{paths, false};
        int part2{sl2.sand()};

        return DayResults{
            "There are " + std::to_string(part1) + " sands when the abyss begins...",
            "Wait, am we floating?? Oh, there are " + std::to_string(part2) + " sands for part 2."
        };
    }
}