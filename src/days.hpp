#pragma once
#include <string>
#include "./util.hpp"

// Is this better? Idunno. Having an almost identical hpp for each day
// seems pretty redundant.
#define DAY(n) namespace day##n { DayResults run(std::string filename); }

DAY(1)
DAY(2)
DAY(3)
DAY(4)
DAY(5)
DAY(6)
DAY(7)
DAY(8)
DAY(9)
DAY(10)
DAY(11)
DAY(12)

#undef DAY