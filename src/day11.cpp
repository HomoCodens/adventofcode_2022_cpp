#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "./days.hpp"
#include "./util.hpp"

namespace day11 {
    using std::deque;
    using std::string;
    using std::vector;

    namespace {
        // Gut tells us to do it functional like, but when in OO-Land...
        
        // Still need a bigger type than int here, worst case is about 96000**2
        using Worry = ulong;

        struct Throw {
            int recipient{0};
            Worry item{0};
        };

        struct MonkeOperand {
            bool isConst{true};
            Worry value{0};
        };

        class Operation {
            private:
                char m_operator{'+'};
                MonkeOperand m_leftOperand;
                MonkeOperand m_rightOperand;

            public:
                Operation(string left, string right, char op) : m_operator{op} {
                    if(left == "old") {
                        m_leftOperand = MonkeOperand{false};
                    } else {
                        Worry leftInt;
                        std::istringstream ss{left};
                        ss >> leftInt;
                        m_leftOperand = MonkeOperand{true, leftInt};
                    }

                    if(right == "old") {
                        m_rightOperand = MonkeOperand{false};
                    } else {
                        Worry rightInt;
                        std::istringstream ss{right};
                        ss >> rightInt;
                        m_rightOperand = MonkeOperand{true, rightInt};
                    }
                }

                Worry apply(Worry old) {
                    Worry a{m_leftOperand.isConst ? m_leftOperand.value : old};
                    Worry b{m_rightOperand.isConst ? m_rightOperand.value : old};

                    switch(m_operator) {
                        case '+':
                            return a + b;
                        case '*':
                            return a * b;
                    }
                }

                void print() {
                    std::cout << "Operation: new = "
                                << (m_leftOperand.isConst ? std::to_string(m_leftOperand.value) : "old")
                                << ' ' << m_operator << ' '
                                << (m_rightOperand.isConst ? std::to_string(m_rightOperand.value) : "old")
                                << '\n';
                }
        };

        class Monke {
            private:
                deque<Worry> m_items{};
                Worry m_testDivisor{0};
                int m_throwToIfTrue{0};
                int m_throwToIfFalse{0};
                Operation m_inspection{"3", "3", '+'};
                int m_nInspections{0};

            public:
                Monke(vector<string> lines) {
                    string _s{""};
                    char _c{'\0'};

                    std::istringstream items{lines[1]};
                    // Starting items:
                    items >> _s >> _s;
                    int item{0};
                    while(!items.eof()) {
                        item = 0;
                        items >> item >> _c;
                        if(item != 0) {
                            m_items.push_back(item);
                        }
                    }

                    std::istringstream inspec{lines[2]};
                    // Operation: new = 
                    inspec >> _s >> _s >> _c;

                    string left, right;
                    char op;
                    inspec >> left >> op >> right;

                    m_inspection = Operation{left, right, op};

                    std::istringstream test{lines[3]};
                    // Test: divisible by <m_testDivisor>
                    test >> _s >> _s >> _s >> m_testDivisor;

                    std::istringstream trueBranch{lines[4]};
                    // If true: throw to monkey <m_throwToIfTrue>
                    trueBranch >> _s >> _s >> _s >> _s >> _s >> m_throwToIfTrue;

                    std::istringstream falseBranch{lines[5]};
                    // If false: throw to monkey <m_throwToIfFalse>
                    falseBranch >> _s >> _s >> _s >> _s >> _s >> m_throwToIfFalse;
                }

                bool haz() {
                    return m_items.size() > 0;
                }

                Throw throwNextItem(Worry relief, Worry anxieTea) {
                    ++m_nInspections;

                    Worry item{m_items.front()};
                    m_items.pop_front(); // Not a fan of pops that don't return a value..

                    Worry increasedWorry{m_inspection.apply(item)};

                    // Had typed "> 0" here out of habit, cost me about 2 hours.. :shrug:
                    if(relief > 1) {
                        increasedWorry /= relief;
                    } else {
                        increasedWorry %= anxieTea;
                    }

                    if(increasedWorry % m_testDivisor) {
                        return Throw{m_throwToIfFalse, increasedWorry};
                    } else {
                        return Throw{m_throwToIfTrue, increasedWorry};
                    }
                }

                void catchItem(Worry item) {
                    m_items.push_back(item);
                }

                int getMonkeBeesKnees() {
                    return m_nInspections;
                }

                int getDivisor() {
                    return m_testDivisor;
                }

                void print() {
                    std::cout << "Items: ";
                    for(int i : m_items) {
                        std::cout << i << ", ";
                    }
                    std::cout << '\n';

                    m_inspection.print();

                    std::cout << "Test: divisible by " << m_testDivisor << '\n';

                    std::cout << "  If true: throw to monkey " << m_throwToIfTrue << '\n';
                    std::cout << "  If false: throw to monkey " << m_throwToIfFalse << '\n';

                    std::cout << "ook!\n\n";
                }
        };

        long runMonkesFor(vector<Monke> monkes, int n, int relief) {
            Worry anxieTea{1};

            for(Monke m : monkes) {
                anxieTea *= m.getDivisor();
            }

            for(int r = 0; r < n; ++r) {
                for(Monke& m : monkes) {
                    while(m.haz()) {
                        Throw t{m.throwNextItem(relief, anxieTea)};
                        monkes[t.recipient].catchItem(t.item);
                    }
                }

                /*
                Fer de debugging
                if(relief == 1 && (r == 0 || r == 19 || (r + 1) % 1000 == 0)) {
                    std::cout << "==== After round " << (r+1) << " ====\n";
                    for(Monke m : monkes) {
                        std::cout << m.getMonkeBeesKnees() << '\n';
                        m.print();
                    }
                    std::cout << '\n';
                }
                */
            }

            vector<long> throws{};
            for(Monke m : monkes) {
                throws.push_back(m.getMonkeBeesKnees());
            }
            std::sort(throws.begin(), throws.end(), std::greater{});

            return throws[0] * throws[1];
        }
    }

    DayResults run(string filename) {
        std::ifstream f{filename};

        vector<Monke> monkes{};

        vector<string> monkeLines{};
        string line{""};
        while(!f.eof()) {
            std::getline(f, line);
            while(line != "") {
                monkeLines.push_back(line);
                std::getline(f, line);
            }
            monkes.push_back(Monke{monkeLines});
            monkeLines = {};
        }

        long part1{runMonkesFor(monkes, 20, 3)};

        long part2{runMonkesFor(monkes, 10000, 1)};

        return DayResults{
            "After the 20 rounds, two busiest munkes had " + std::to_string(part1),
            "After the 10000 rounds, two busiest munkes had " + std::to_string(part2)
        };
    }
}