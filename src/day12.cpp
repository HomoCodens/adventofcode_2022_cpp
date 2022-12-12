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
            uint g{0};
        };

        struct NodeCompare {
            size_t operator()(const Node& x, const Node& y) const {
                return x.g > y.g;
            }
        };

        bool operator==(const Node& a, const Node& b) {
            return a.p == b.p;
        }

        std::ostream& operator<<(std::ostream& os, const Node& n) {
            os << "Node(" << n.p << ", f: " << n.g << ")";
            return os;
        }

        std::ostream& operator<<(std::ostream& os, const Map& m) {
            for(string l : m) {
                os << l << '\n';
            }

            return os;
        }

        std::vector<Vector2D> getNeighbours(const Vector2D& p, const Map& m, const bool downwards = false) {
            std::vector<Vector2D> out{};
            char heightAt{m[p.y][p.x]};

            for(int offTheSet = -1; offTheSet < 2; offTheSet += 2) {
                int r2 = p.y + offTheSet;
                // Invert the sign of the vertical distance when moving downwards
                if(r2 >= 0 && r2 < m.size() && (1 - 2*downwards)*(m[r2][p.x] - heightAt) <= 1) {
                    out.push_back(Vector2D{p.x, r2});
                }

                int c2 = p.x + offTheSet;
                if(c2 >= 0 && c2 < m[0].size() && (1 - 2*downwards)*(m[p.y][c2] - heightAt) <= 1) {
                    out.push_back(Vector2D{c2, p.y});
                }
            }

            return out;
        }

        // Upon reflection, the swirlyness of the mountain kinda defeats the "star" part..
        int walkies(const Map& map, const Vector2D& start, const Vector2D& goal, bool part1 = true) {
            // Can you smell the wiki? ;P
            std::priority_queue<Node, std::vector<Node>, NodeCompare> edge;
            std::unordered_map<Vector2D, int, PointHasher> gScore;

            if(part1) {
                edge.push(Node{start, 0});
                gScore[start] = 0;
            } else {
                edge.push(Node{goal, 0});
                gScore[goal] = 0;
            }

            while(!edge.empty()) {
                Node at{edge.top()};

                if((part1 && at.p == goal)
                    || (!part1 && map[at.p.y][at.p.x] == 'a')) {
                    return at.g;
                }

                edge.pop();
                for(Vector2D n : getNeighbours(at.p, map, !part1)) { // !part1, !confusingAtAll
                    int newG = gScore.at(at.p) + 1;

                    if(gScore.find(n) == gScore.end() || newG < gScore.at(n)) {
                        gScore[n] = newG;

                        edge.push(Node{n, static_cast<uint>(newG)});
                    }
                }
            }

            return -1;
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

        
        int part1{walkies(map, start, goal)};
        int part2{walkies(map, start, goal, false)};

        return DayResults{
            std::to_string(part1),
            std::to_string(part2)
        };
    }
}