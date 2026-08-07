#pragma once

#include <cmath>
#include <iostream>

namespace test {
    inline int failures = 0;

    inline int report()
    {
        if (failures == 0)
            return 0;
        std::cerr << failures << " check(s) FAILED" << std::endl;
        return 1;
    }
} // namespace test

#define EQ(a, b)                                                              \
    if ((a) != (b)) {                                                         \
        ++test::failures;                                                     \
        std::cerr << "FAILED: " << #a << " != " << #b << " (line " << __LINE__ << ")" << std::endl; \
    }

#define FLOAT_EQ(a, b)                                                        \
    if (std::fabs((a) - (b)) >= 1e-5f) {                                      \
        ++test::failures;                                                     \
        std::cerr << "FAILED: " << #a << " != " << #b << " (line " << __LINE__ << ")" << std::endl; \
    }
