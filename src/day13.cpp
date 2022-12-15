#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "./days.hpp"
#include "./util.hpp"

namespace day13 {
    using std::string;
    using std::vector;

    namespace {
        class PacketThing {
            private:
                bool m_isMarker{false};
            public:
                virtual bool lessThan(PacketThing* other) = 0;
                virtual string getType() = 0;

                void makeMarker() {
                    m_isMarker = true;
                }

                bool isMarker() {
                    return m_isMarker;
                }
        };

        class PacketInteger : public PacketThing {
            private:
                int m_value;

            public:
                PacketInteger(int value) : m_value{value} {}

                string getType() override {
                    return "Integer";
                }

                bool lessThan(PacketThing* other);

                friend std::ostream& operator<<(std::ostream& os, const PacketInteger& x);
        };

        std::ostream& operator<<(std::ostream& os, const PacketInteger& x) {
            os << x.m_value;

            return os;
        }

        class PacketList : public PacketThing {
            private:
                vector<PacketThing*> m_items;
            public:
                PacketList() : m_items{{}} {}
                PacketList(vector<PacketThing*> items) : m_items{items} {}
                PacketList(std::istringstream& ss) : m_items{{}} {
                    // First char is our opening bracket
                    ss.ignore(1);
                    
                    vector<PacketThing*> items{};
                    while(!ss.eof()) {
                        char nextChar{ss.peek()};
                        if(nextChar == '[') {
                            items.push_back(new PacketList(ss));
                        } else if(nextChar == ']') {
                            ss.ignore(1);
                            m_items = items;
                            return;
                        } else if(nextChar == ',') {
                            ss.ignore(1);
                        } else {
                            int i;
                            ss >> i;
                            PacketInteger* pii = new PacketInteger(i);
                            items.push_back(pii);
                        }
                    }
                }

                string getType() override {
                    return "List";
                }

                bool lessThan(PacketThing* other) {
                    if(other->getType() == "Integer") {
                        PacketInteger* o{dynamic_cast<PacketInteger*>(other)};
                        // std::cout << "Comparing " << *this << " to " << *o << '\n';
                        if(m_items.size() == 0) {
                            return true;
                        } else {
                            vector<PacketThing*> q{};
                            q.push_back(other);
                            return this->lessThan(new PacketList(q));
                        }
                    } else {
                        PacketList* o{dynamic_cast<PacketList*>(other)};

                        // std::cout << "Comparing " << *this << " to " << *o << '\n';

                        if(m_items.size() == 0 && o->m_items.size() == 0) {
                            return false;
                        }

                        if(m_items.size() == 0) {
                            return true;
                        }

                        if(o->m_items.size() == 0) {
                            return false;
                        }

                        auto tIt{m_items.begin()};
                        auto oIt{o->m_items.begin()};
                        while(true) {
                            if((*tIt)->lessThan(*oIt)) {
                                return true;
                            }

                            if((*oIt)->lessThan(*tIt)) {
                                return false;
                            }

                            ++tIt;
                            ++oIt;

                            if(tIt == m_items.end() && oIt == o->m_items.end()) {
                                return false;
                            }

                            if(tIt == m_items.end()) {
                                return true;
                            }

                            if(oIt == o->m_items.end()) {
                                return false;
                            }
                        }
                    }
                }

                friend std::ostream& operator<<(std::ostream& os, PacketList& pl);
        };

        std::ostream& operator<<(std::ostream& os, PacketList& pl) {
            os << '[';
            for(PacketThing* pt : pl.m_items) {
                if(pt->getType() == "Integer") {
                    os << *(dynamic_cast<PacketInteger*>(pt)) << ',';
                } else {
                    os << *(dynamic_cast<PacketList*>(pt));
                }
            }
            os << ']';
            return os;
        }

        bool PacketInteger::lessThan(PacketThing* other) {
            if(other->getType() == "Integer") {
                PacketInteger* o{dynamic_cast<PacketInteger*>(other)};
                return m_value < o->m_value;
            } else {
                vector<PacketThing*> q{};
                q.push_back(this);
                PacketList qq{q};
                return qq.lessThan(other);
            }
        }

        bool operator<(PacketList p1, PacketList p2) {
            PacketThing* p2p{&p2};
            return p1.lessThan(p2p);
        }
    }

    DayResults run(std::string filename) {
        std::ifstream f{filename};

        vector<PacketList> packets{};

        int part1{0};
        int grp{1};
        while(!f.eof()) {
            string s1;
            string s2;
            f >> s1 >> s2;

            if(s1 != "" && s2 != "") {
                std::istringstream ss1{s1};
                std::istringstream ss2{s2};

                PacketList p1{ss1};
                PacketList p2{ss2};

                bool isLess{p1.lessThan(&p2)};

                // std::cout << p1 << "\nvs\n" << p2 << '\n';
                // std::cout << isLess << "\n\n";

                if(isLess) {
                    part1 += grp;
                }
                ++grp;

                packets.push_back(p1);
                packets.push_back(p2);
            }
        }

        std::istringstream m1s{"[[2]]"};
        PacketList mark1{m1s};
        PacketThing* m1p{&mark1};
        m1p->makeMarker();
        packets.push_back(mark1);
        std::istringstream m2s{"[[6]]"};
        PacketList mark2{m2s};
        PacketThing* m2p{&mark2};
        m2p->makeMarker();
        packets.push_back(mark2);

        std::sort(packets.begin(), packets.end());

        int part2{1};
        int q{1};
        for(PacketList pl : packets) {
            if(pl.isMarker()) {
                part2 *= q;
            }
            ++q;
        }

        return DayResults{
            std::to_string(part1),
            std::to_string(part2)
        };
    }
}

/*
Comparing [[[[][4,2,]1,[9,9,][10,2,5,2,10,]][]3,][[[10,4,2,3,3,][]3,9,[6,]]10,4,][5,[[8,8,][]9,]9,[[7,8,]3,[]3,]][[4,[3,8,]10,7,][][9,]7,[5,[]5,1,[0,3,5,10,10,]]][[[9,4,][3,1,2,8,]10,][[6,4,5,]]]] to [[9,]]
Comparing [[[][4,2,]1,[9,9,][10,2,5,2,10,]][]3,] to [9,]
Comparing [[][4,2,]1,[9,9,][10,2,5,2,10,]] to 9
Comparing [[][4,2,]1,[9,9,][10,2,5,2,10,]] to [9,]
Comparing [] to 9
Comparing [9,] to []
Comparing [9,] to [[][4,2,]1,[9,9,][10,2,5,2,10,]]
Comparing [9,] to []
Comparing [] to 9
Comparing [9,] to [[[][4,2,]1,[9,9,][10,2,5,2,10,]][]3,]
Comparing [9,] to [[][4,2,]1,[9,9,][10,2,5,2,10,]]
Comparing [9,] to []
Comparing [] to 9
[[[[][4,2,]1,[9,9,][10,2,5,2,10,]][]3,][[[10,4,2,3,3,][]3,9,[6,]]10,4,][5,[[8,8,][]9,]9,[[7,8,]3,[]3,]][[4,[3,8,]10,7,][][9,]7,[5,[]5,1,[0,3,5,10,10,]]][[[9,4,][3,1,2,8,]10,][[6,4,5,]]]]
vs
[[9,]]
0
*/

/*
I'll be back for yall...

        class PacketThing {
            public:
                virtual std::vector<std::unique_ptr<PacketThing>>::const_iterator begin() = 0;
                virtual std::vector<std::unique_ptr<PacketThing>>::const_iterator end() = 0;

                bool operator<(PacketThing& other) {
                    auto thisIt{this->begin()};
                    auto thisE{this->end()};
                    auto otherIt{other.begin()};
                    auto otherE{other.end()};

                    while(true) {
                        if(*thisIt < *otherIt) {
                            return true;
                        }

                        if(*thisIt > *otherIt) {
                            return false;
                        }

                        if(++thisIt == thisE) {
                            return true;
                        }

                        if(++otherIt == otherE) {
                            return false;
                        }
                    }
                }

                friend bool operator<(int lhs, PacketThing& rhs);
                friend bool operator<(PacketThing& lhs, int rhs);
        };

        bool operator<(int lhs, PacketThing& rhs) {
            return PacketInteger{lhs} < rhs;
        }

        bool operator<(PacketThing& lhs, int rhs) {
            return lhs < PacketInteger{rhs};
        }

        class PacketList : public PacketThing {
            private:
                std::vector<std::unique_ptr<PacketThing>> m_items{};
                friend class PacketInteger;
            public:
                void push(std::unique_ptr<PacketThing> ptr) {
                    m_items.push_back(ptr);
                }

                void push(PacketInteger x) {
                    push(std::make_unique<PacketThing>(x));
                }

                void push(PacketList x) {
                    push(std::make_unique<PacketThing>(x));
                }

                bool operator<(PacketList& other) {
                    auto myIt{m_items.begin()};
                    auto otherIt{other.m_items.begin()};

                    while(true) {
                        if(**myIt < **otherIt) {
                            return true;
                        }

                        if(**otherIt < **myIt) {
                            return false;
                        }

                        ++myIt;
                        ++otherIt;

                        if(otherIt == other.m_items.end() && myIt == m_items.end()) {
                            return false; // ?
                        } else if(otherIt == other.m_items.end()) {
                            return false;
                        } else if(myIt == m_items.end()) {
                            return true;
                        }
                    }
                }

                bool operator<(PacketInteger& other) {
                    if(m_items.size() > 0) {
                        return other.m_value < *m_items[0];
                    } else {
                        return false;
                    }
                }

                std::vector<std::unique_ptr<PacketThing>>::const_iterator begin() override {
                    return m_items.cbegin();
                }

                std::vector<std::unique_ptr<PacketThing>>::const_iterator end() override {
                    return m_items.cend();
                }

                // Friendzone
                friend std::ostream& operator<<(std::ostream& os, const PacketList& pl);
        };

        class PacketInteger : public PacketThing {
            private:
                int m_value;

                friend class PacketList;
            public:
                PacketInteger(int value) : m_value{value} {}
                PacketInteger(char value) : m_value{value - '0'} {}

                bool operator<(PacketList& other) {
                    if(other.m_items.size() > 0) {
                        return !(*other.m_items[0] < m_value);
                    } else {
                        return false;
                    }
                }

                bool operator<(PacketInteger& other) {
                    return m_value < other.m_value;
                }
        };
*/