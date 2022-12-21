#include <algorithm>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
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
    using std::queue;

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
        using DistanceMap = unordered_map<string, unordered_map<string, int>>;

        class TheGame {
            private:
                Adjacency m_map;
                DistanceMap m_distances {};

                void findDistance(string from, string to) {
                    queue<string> edge;
                    set<string> done;
                    edge.push(from);

                    while(!edge.empty()) {
                        string at{edge.front()};
                        if(at == to) {
                            return;
                        }

                        edge.pop();
                        for(string n : m_map[at].getNeighbours()) {
                            if(!done.contains(n)) {
                                int distToNei{m_distances[std::min(from, at)][std::max(from, at)] + 1};
                                m_distances[std::min(from, n)][std::max(from, n)] = distToNei;
                                edge.push(n);
                            }
                        }
                        done.insert(at);
                    }

                    throw "Oi, this not supposed to happen!";
                }

                int getDistance(string from, string to) {
                    if(m_distances[std::min(from, to)][std::max(from, to)] == 0) {
                        findDistance(from, to);
                    }

                    return m_distances[std::min(from, to)][std::max(from, to)];
                }

                int recur(int currentPlayer,
                            int nPlayers,
                            vector<string> ats,
                            vector<string> queue,
                            vector<int> remainingTime,
                            int runningTotal) {
                    if(queue.size() == 0) {
                        std::cout << "\nThe total be " << runningTotal << "\n\n";
                        return runningTotal;
                    }
                    
                    int best{runningTotal};
                    vector<int> nrt{remainingTime};
                    vector<string> nats{ats};

                    for(string next : queue) {
                        int distToNext{getDistance(ats[currentPlayer], next)};

                        // We still have time to get there and open the valve
                        if(distToNext < remainingTime[currentPlayer]) {
                            // There has to be a petter way (meow?)

                            vector<string> nq{queue};
                            nq.erase(std::remove(nq.begin(), nq.end(), next), nq.end());

                            int timeRemainingWhenValveOpen{remainingTime[currentPlayer] - distToNext - 1};

                            nrt[currentPlayer] = timeRemainingWhenValveOpen;
                            
                            nats[currentPlayer] = next;

                            int candidate{recur((currentPlayer + 1) % nPlayers,
                                                nPlayers,
                                                nats,
                                                nq,
                                                nrt,
                                                runningTotal + m_map[next].getFlowRate()*timeRemainingWhenValveOpen)};
                            if(best < candidate) {
                                best = candidate;
                            }
                        }
                    }

                    return best;
                }

            public:
                TheGame(Adjacency a) : m_map{a} {};

                int maximIce() {
                    vector<string> interestingNodes;
                    for(auto i : m_map) {
                        if(i.second.getFlowRate() > 0) {
                            interestingNodes.push_back(i.second.getId());
                        }
                    }

                    vector<string> startPos;
                    startPos.push_back("AA");

                    vector<int> timeBudgets;
                    timeBudgets.push_back(30);
                    return recur(0, 1, startPos, interestingNodes, timeBudgets, 0);
                }

                int maximTwice() {
                    vector<string> interestingNodes;
                    for(auto i : m_map) {
                        if(i.second.getFlowRate() > 0) {
                            interestingNodes.push_back(i.second.getId());
                        }
                    }

                    vector<string> startPos;
                    startPos.push_back("AA");
                    startPos.push_back("AA");

                    vector<int> timeBudgets;
                    timeBudgets.push_back(26);
                    timeBudgets.push_back(26);
                    return recur(0, 2, startPos, interestingNodes, timeBudgets, 0);
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

        TheGame tg{a};
        int part1{tg.maximIce()};
        std::cout << "part 1 done...\n";
        int part2{0};
        part2 = tg.maximTwice();
        return DayResults{
            std::to_string(part1),
            std::to_string(part2)
        };
    }
}