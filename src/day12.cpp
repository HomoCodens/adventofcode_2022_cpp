#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <queue>
#include <unordered_map>
#include <vector>

#include "./days.hpp"
#include "./util.hpp"
#include "./vector2d.hpp"

namespace day12 {
    using std::string;
    using Map = std::vector<string>;

    namespace {

        struct Node {
            Vector2D p{0, 0};
            uint f{0};
        };

        struct NodeCompare {
            size_t operator()(const Node& x, const Node& y) const {
                return x.f > y.f;
            }
        };

        bool operator==(const Node& a, const Node& b) {
            return a.p == b.p;
        }

        std::ostream& operator<<(std::ostream& os, const Node& n) {
            os << "Node(" << n.p << ", f: " << n.f << ")";
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const Map& m) {
            for(string l : m) {
                os << l << '\n';
            }

            return os;
        }

        std::vector<Vector2D> getNeighbours(const Vector2D& p, const Map& m) {
            std::vector<Vector2D> out{};
            char heightAt{m[p.y][p.x]};

            for(int offTheSet = -1; offTheSet < 2; offTheSet += 2) {
                int r2 = p.y + offTheSet;
                if(r2 >= 0 && r2 < m.size() && (m[r2][p.x] - heightAt) <= 1) {
                    out.push_back(Vector2D{p.x, r2});
                }

                int c2 = p.x + offTheSet;
                if(c2 >= 0 && c2 < m[0].size() && (m[p.y][c2] - heightAt) <= 1) {
                    out.push_back(Vector2D{c2, p.y});
                }
            }

            return out;
        }

        // Upon reflection, the swirlyness of the mountain kinda defeats the "star" part..
        int aStar(const Map& map, const Vector2D& start, const Vector2D& goal) {
            // Can you smell the wiki? ;P
            std::priority_queue<Node, std::vector<Node>, NodeCompare> edge;
            edge.push(Node{start, manhattanDist(start, goal)});

            std::unordered_map<Vector2D, int, PointHasher> gScore;
            gScore[start] = 0;

            std::unordered_map<Vector2D, Vector2D, PointHasher> parent;

            //std::cout << "Ma goal be " << goal << '\n';
            std::cout << "Ma start be " << start << '\n';
            while(!edge.empty()) {
                Node at{edge.top()};

                //std::cout << "Now lookings at " << at.p << '\n';

                if(at.p == goal) {
                    std::cout << "aoooga!\n";
                    std::cout << "Got " << at.f << " many steps.\n";
                    return at.f;
                }

                edge.pop();
                for(Vector2D n : getNeighbours(at.p, map)) {
                    int newG = gScore.at(at.p) + 1;

                    if(gScore.find(n) == gScore.end() || newG < gScore.at(n)) {
                        gScore[n] = newG;

                        parent[n] = at.p;

                        // manhattan dist is(?) consistent, therefore no need to worry about dupes
                        edge.push(Node{n, newG + manhattanDist(n, goal)});
                    }
                }
            }

            return -1;
        }

        void printPath(std::unordered_map<Vector2D, Vector2D, PointHasher> parents, Vector2D goal) {
            Vector2D at{goal};
            while(parents.find(at) != parents.end()) {
                std::cout << at << '\n';
                at = parents.at(at); // at At at atat. ATAT!
            }
        }
    }

    DayResults run(string filename) {
        Vector2D start;
        Vector2D goal;
        Map map{};

        string line{""};
        int row{0};
        std::ifstream f{filename};
        while(!f.eof()) {
            line = "";
            std::getline(f, line);
            if(line != "") {
                auto startCol{line.find("S")};
                if(startCol != string::npos) {
                    start = Vector2D{static_cast<int>(startCol), row};
                    line[startCol] = 'a';
                }

                auto endCol{line.find("E")};
                if(endCol != string::npos) {
                    goal = Vector2D{static_cast<int>(endCol), row};
                    line[endCol] = 'z';
                }

                map.push_back(line);
                ++row;
            }
        }

        
        int part1{aStar(map, start, goal)};

        // Got much better idea, but in hurry and want star
        int part2{1000000};
        for(int r = 0; r < map.size(); ++r) {
            for(int c = 0; c < map[0].size(); ++c) {
                if(map[r][c] == 'a') {
                    int candidate{aStar(map, Vector2D{c, r}, goal)};
                    if(candidate >= 0 && candidate < part2) {
                        part2 = candidate;
                    }
                }
            }
        }

        return DayResults{
            std::to_string(part1),
            std::to_string(part2)
        };
    }
}