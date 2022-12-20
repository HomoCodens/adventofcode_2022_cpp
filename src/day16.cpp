#include <iostream>
#include <fstream>
#include <unordered_map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "./days.hpp"
#include "./util.hpp"

namespace day16 {
    using std::string;
    using std::unordered_map;
    using std::vector;
    using std::set;

    namespace {
        class Node {
            private:
                string id;
                int flowRate;
                vector<string> neighbours;

            public:
                Node() : id{""}, flowRate{-1} {}

                Node(string line) {
                    // Valve <[A-Z]{2}> has flow rate=<int>; tunnels lead to valve(s) DD, II, BB
                    std::istringstream ss{line};

                    string _;
                    ss >> _ >> id;
                    ss.ignore(15);
                    ss >> flowRate;
                    ss.ignore(24);
                    while(!ss.eof()) {
                        string n;
                        ss >> n;
                        if(n != "") {
                            neighbours.push_back(n.substr(0, n.find_first_of(',')));
                        }
                    }
                }

                string getId() {
                    return id;
                }

                int getFlowRate() {
                    return flowRate;
                }

                vector<string>& getNeighbours() {
                    return neighbours;
                }

                friend std::ostream& operator<<(std::ostream& os, const Node& n);
        };

        std::ostream& operator<<(std::ostream& os, const Node& n) {
            os << "Node " << n.id << " (fr= " << n.flowRate << "); neighbours ";
            for(string nn : n.neighbours) {
                os << nn << ' ';
            }

            return os;
        }

        using Adjacency = unordered_map<string, Node>;

        class TheGame {
            private:
                Adjacency m_map;

                int findBestWorker(string at,
                                    int untappedProduction,
                                    int runningTotal = 0,
                                    int bestSoFar = 0,
                                    int timeRemaining = 30,
                                    set<string> openedValves = set<string>{}) {
                    if(timeRemaining == 0) {
                        int bst{std::max(bestSoFar, runningTotal)};
                        std::cout << "Outta time, returning " << bst << '\n';
                        return bst;
                    }

                    if(untappedProduction == 0) {
                        int bst{std::max(bestSoFar, runningTotal)};
                        std::cout << "Outta vents, returning " << bst << '\n';
                        return bst;
                    }

                    // Dead end, we can't match bestSoFar even at instant max production
                    if(untappedProduction*(timeRemaining - 1) <= (bestSoFar - runningTotal)) {
                        int bst{std::max(bestSoFar, runningTotal)};
                        //std::cout << "Outta steam, returning " << bst << '\n';
                        return bst;
                    }

                    //if(timeRemaining == 15) {
                        //std::cout << "sniffing around " << at << '\n';
                    //}

                    Node currentNode{m_map[at]};

                    int myBest{bestSoFar};
                    if(currentNode.getFlowRate() > 0 && openedValves.find(at) == openedValves.end()) {
                        set<string> newOpened{openedValves};
                        newOpened.insert(at);
                        int withOpened{findBestWorker(at,
                                                untappedProduction - currentNode.getFlowRate(),
                                                runningTotal + (timeRemaining - 1)*currentNode.getFlowRate(),
                                                bestSoFar,
                                                timeRemaining - 1,
                                                newOpened)};
                        if(withOpened > myBest) {
                            myBest = withOpened;
                        }
                    }

                    for(string neighbour : currentNode.getNeighbours()) {
                        int b2 = findBestWorker(neighbour, untappedProduction, runningTotal, myBest, timeRemaining - 1, openedValves);
                        if(b2 > myBest) {
                            myBest = b2;
                        }
                    }

                    return myBest;
                }

            public:
                TheGame(Adjacency a) : m_map{a} {};

                int findBest() {
                    int productionPotential{0};
                    for(auto node : m_map) {
                        productionPotential += node.second.getFlowRate();
                    }

                    return findBestWorker("AA", productionPotential);
                }

        };
    }


    DayResults run(string filename) {
        Adjacency a;
        std::ifstream f{filename};
        while(!f.eof()) {
            string line;
            std::getline(f, line);

            if(line != "") {
                Node n{line};
                a.insert_or_assign(n.getId(), n);
            }
        }

        for(auto i : a) {
            std::cout << i.second << '\n';
        }

        TheGame tg{a};
        int part1{tg.findBest()};

        return DayResults{
            std::to_string(part1)
        };
    }
}