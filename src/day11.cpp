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
        
        /*class MonkeOperand {
            public:
                virtual int getValue(int old) {
                    return 0;
                };
        };

        class ConstOperand : public MonkeOperand {
            private:
                int m_value{0};

            public:
                ConstOperand(int value) : m_value{value} {
                }

                int getValue(int old) override {
                    return m_value;
                }
        };

        std::ostream operator<<(std::ostream& os, const ConstOperand& o) {
            os << o.getValue(0);
        }

        class OldOperand : public MonkeOperand {
            int getValue(int old) override {
                return old;
            }
        };

        std::ostream operator<<(std::ostream& os, const OldOperand& o) {
            os << "old";
        }*/

        struct Throw {
            int recipient{0};
            int item{0};
        };

        struct MonkeOperand {
            bool isConst{true};
            int value{0};
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
                        m_leftOperand = MonkeOperand{true, std::atoi(left.c_str())};
                    }

                    if(right == "old") {
                        m_rightOperand = MonkeOperand{false};
                    } else {
                        m_rightOperand = MonkeOperand{true, std::atoi(right.c_str())};
                    }
                }

                int apply(int old) {
                    int a{m_leftOperand.isConst ? m_leftOperand.value : old};
                    int b{m_rightOperand.isConst ? m_rightOperand.value : old};

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
                deque<int> m_items{};
                int m_testDivisor{0};
                int m_throwToIfTrue{0};
                int m_throwToIfFalse{0};
                Operation m_inspection{"3", "3", '+'};
                int m_nInspections{0};

            public:
                Monke(vector<string> lines) {

                    //assert(lines.size() == 6);

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

                Throw throwNextItem() {
                    ++m_nInspections;

                    int item{m_items.front()};
                    m_items.pop_front(); // Not a fan of pops that don't return a value..

                    int increasedWorry{m_inspection.apply(item) / 3};
                    if(increasedWorry % m_testDivisor) {
                        return Throw{m_throwToIfFalse, increasedWorry};
                    } else {
                        return Throw{m_throwToIfTrue, increasedWorry};
                    }
                }

                void catchItem(int item) {
                    m_items.push_back(item);
                }

                int getMonkeBeesKnees() {
                    return m_nInspections;
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

        for(int r = 0; r < 20; ++r){
            for(Monke& m : monkes) {
                while(m.haz()) {
                    Throw t{m.throwNextItem()};
                    monkes[t.recipient].catchItem(t.item);
                }
            }
        }

        for(Monke m : monkes) {
            m.print();
        }

        vector<int> throws{};
        for(Monke m : monkes) {
            throws.push_back(m.getMonkeBeesKnees());
        }
        std::sort(throws.begin(), throws.end(), std::greater{});

        return DayResults{
            std::to_string(throws[0] * throws[1])
        };
    }
}