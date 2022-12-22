#include <cmath>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <utility>
#include <vector>

#include "./days.hpp"
#include "./util.hpp"
#include "./vector2d.hpp"

namespace day17 {
    using std::string;
    using std::unordered_map;
    using std::unordered_set;
    using std::vector;

    namespace {
        class Peece {
            private:
                vector<Vector2D> m_parts;
            
            public:
                Peece(vector<Vector2D> parts) : m_parts{parts} {}

                vector<Vector2D> getPositions(Vector2D piecePos) {
                    vector<Vector2D> out;
                    for(Vector2D o : m_parts) {
                        out.push_back(o + piecePos);
                    }

                    return out;
                }
        };

        class Wind {
            private:
                int m_pos{0};
                string m_buffer;
            
            public:
                Wind(string w) : m_buffer{w} {}

                Vector2D next() {
                    Vector2D out;
                    if(m_buffer[m_pos] == '>') {
                        out = Vector2D(1, 0);
                    } else {
                        out = Vector2D(-1, 0);
                    }

                    m_pos = (m_pos + 1) % m_buffer.size();

                    return out;
                }

                int getPosition() {
                    return m_pos;
                }
        };

        using PointMap = unordered_set<Vector2D, PointHasher>;

        class Verld {
            private:
                PointMap m_map;
                Wind m_wind;
                vector<Peece> m_pieces;
                int m_currentPiece{0};
                Vector2D m_currentPiecePos;
                int m_yMax{0};
                Vector2D down{Vector2D(0, -1)}; // aka 'The Enemy Gate'
                int m_nPieces{0};

                /*Peece nextPiece() {
                    Peece out{m_pieces[m_currentPiece]};
                    ++m_currentPiece;
                    return out;
                }*/

                bool pieceInWall(vector<Vector2D> points) {
                    for(Vector2D x : points) {
                        if(x.x < 0 || x.x > 6) {
                            return true;
                        }
                    }

                    return false;
                }

                bool pieceCollides(vector<Vector2D> points) {
                    for(Vector2D x : points) {
                        if(m_map.find(x) != m_map.end()) {
                            return true;
                        }
                    }

                    return false;
                }

                bool pieceInFloor(vector<Vector2D> points) {
                    for(Vector2D x : points) {
                        if(x.y < 0) {
                            return true;
                        }
                    }

                    return false;
                }

                bool attemptMove(Vector2D v) {
                    m_currentPiecePos = m_currentPiecePos + v;
                    vector<Vector2D> points{m_pieces[m_currentPiece].getPositions(m_currentPiecePos)};

                    /*for(Vector2D x : points) {
                        std::cout << x << ' ';
                    }
                    std::cout << "\nAfter movin " << v << '\n';

                    if(pieceInFloor(points)) {
                        std::cout << "Too low!\n";
                    } else if(pieceInWall(points)) {
                        std::cout << "Bonk!\n";
                    } else if(pieceCollides(points)) {
                        std::cout << "Crashed into another...\n";
                    } else {
                        std::cout << "All good here.\n";
                    }*/

                    if(pieceInFloor(points) || pieceInWall(points) || pieceCollides(points)) {
                        m_currentPiecePos = m_currentPiecePos - v;
                        return false;
                    }

                    return true;
                }

                void dropPiece() {
                    m_currentPiecePos = Vector2D(2, m_yMax + 3);

                    while(true) {
                        Vector2D gush{m_wind.next()};

                        attemptMove(gush);

                        if(!attemptMove(down)) {
                            break;
                        }
                    }

                    for(Vector2D x : m_pieces[m_currentPiece].getPositions(m_currentPiecePos)) {
                        m_map.insert(x);
                        if(x.y >= m_yMax) {
                            m_yMax = x.y + 1;
                        }
                    }

                    ++m_nPieces;
                    m_currentPiece = (m_currentPiece + 1) % m_pieces.size();
                }

            public:
                Verld(string w) : m_wind{Wind{w}} {
                    // heard a utuber say 'horizontical' recently ;P
                    vector<Vector2D> horizonticalIPieces{{Vector2D(0, 0),
                                                          Vector2D(1, 0),
                                                          Vector2D(2, 0),
                                                          Vector2D(3, 0)}};
                    vector<Vector2D> plusPieces{{Vector2D(0, 1),
                                                 Vector2D(1, 0),
                                                 Vector2D(1, 1),
                                                 Vector2D(1, 2),
                                                 Vector2D(2, 1)}};
                    vector<Vector2D> lThingPieces{{Vector2D(0, 0),
                                                   Vector2D(1, 0),
                                                   Vector2D(2, 0),
                                                   Vector2D(2, 1),
                                                   Vector2D(2, 2)}};
                    vector<Vector2D> verticalIPieces{{Vector2D(0, 0),
                                                      Vector2D(0, 1),
                                                      Vector2D(0, 2),
                                                      Vector2D(0, 3)}};
                    vector<Vector2D> squarePieces{{Vector2D(0, 0),
                                                   Vector2D(1, 0),
                                                   Vector2D(0, 1),
                                                   Vector2D(1, 1)}};

                    m_pieces = {Peece{horizonticalIPieces},
                                Peece{plusPieces},
                                Peece{lThingPieces},
                                Peece{verticalIPieces},
                                Peece{squarePieces}};
                }

                void showsualize(int m_yMax, std::ostream& os) {
                    for(int y = m_yMax; y >= 0; --y) {
                        os << '|';
                        for(int x = 0; x < 7; ++x) {
                            if(m_map.find(Vector2D(x, y)) != m_map.end()) {
                                os << '#';
                            } else {
                                os << '.';
                            }
                        }
                        os << "|\n";
                    }
                    os << "+-------+\n";
                }

                int playPart1() {
                    for(int i = 0; i < 2022; ++i) {
                        dropPiece();

                        if(i <= 10) {
                            showsualize(m_yMax + 3, std::cout);
                        }
                    }

                    //std::ofstream f{"./piledump.txt"};
                    //showsualize(m_yMax, f);

                    return m_yMax;
                }

                uint64_t playPart2() {
                    std::cout << "Detecting cycle...\n";
                    unordered_map<ulong, std::pair<int, int>> horizonticalBarWinds;
                    ulong horizonticalHash{0}; // "hash"
                    while(m_currentPiece > 0 || horizonticalBarWinds.find(horizonticalHash) == horizonticalBarWinds.end()) {
                        if(m_currentPiece == 0) {
                            horizonticalBarWinds[horizonticalHash] = std::make_pair(m_yMax, m_nPieces);
                            horizonticalHash = m_wind.getPosition();
                        } else {
                            horizonticalHash += m_wind.getPosition()*std::pow(10, m_currentPiece);
                        }

                        dropPiece();
                    }
                    std::cout << "Cycle find!\n";
                    int cyclePeriod{(m_nPieces - horizonticalBarWinds[horizonticalHash].second)};
                    int firstCycle{horizonticalBarWinds[horizonticalHash].first};
                    int superMegaGigaChadPieceHeight{(m_yMax - horizonticalBarWinds[horizonticalHash].first)};
                    std::cout << "Period: " << cyclePeriod << '\n';
                    std::cout << "Begin: " << firstCycle << '\n';
                    std::cout << "deltaHeight: " << superMegaGigaChadPieceHeight << '\n';

                    uint64_t targetNMoves{1000000000000L};
                    uint64_t movesToGo{targetNMoves - m_nPieces};
                    uint64_t cyclesToDo{movesToGo / cyclePeriod};
                    uint64_t movesToDo{movesToGo % cyclePeriod};

                    std::cout << "Advancing another " << movesToDo << " pieces...\n";
                    for(int i = 0; i < movesToDo; ++i) {
                        dropPiece();
                    }

                    return cyclesToDo*superMegaGigaChadPieceHeight + m_yMax;
                }
        };
    }

    DayResults run(string filename) {
        std::ifstream f{filename};
        string gushes;
        std::getline(f, gushes);

        // TODO: Make Verld reusable :shrug:
        Verld world{gushes};
        int part1{world.playPart1()};

        Verld alternateWorld{gushes};
        auto part2{alternateWorld.playPart2()};

        return DayResults{
            "After 2022 pieces, the tower be " + std::to_string(part1) + " tall.",
            "After 1000000000000 pieces (and some trickery), " + std::to_string(part2)
        };
    }
}

/*
Part 2 ideas:
    1) track only floor level
    2) find periodicity
        2.a) after 5*wind.size()?
    3) reduce windshweeps for pieces
*/