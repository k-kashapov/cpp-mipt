#include <cassert>
#include <iostream>
#include <numeric>
#include <utility>

#include <gtest/gtest.h>

int gcd_iterative(int a, int b) {
    while (b != 0) {
        a = std::exchange(b, a % b);
    }
    return a;
}

int gcd_recursive(int a, int b) {
    if (b == 0)
        return a;
    return gcd_recursive(b, a % b);
}

int lcm_custom(int a, int b) {
    if (a == 0 || b == 0)
        return 0;
    return (a * b) / gcd_iterative(a, b);
}

static const int test_cases[][2] = {{12, 18}, {7, 13}, {48, 36}, {0, 5}, {0, 0}, {15, 25}};

TEST(Test09, TestGCD) {
    for (const auto &[a, b] : test_cases) {
        int my_gcd_iter = gcd_iterative(a, b);
        int my_gcd_rec = gcd_recursive(a, b);

        int std_gcd = std::gcd(a, b);

        ASSERT_EQ(my_gcd_iter, std_gcd);
        ASSERT_EQ(my_gcd_rec, std_gcd);
    }
}

TEST(Test09, TestLCM) {
    for (const auto &[a, b] : test_cases) {
        int my_lcm = lcm_custom(a, b);
        int std_lcm = std::lcm(a, b);
        ASSERT_EQ(my_lcm, std_lcm);
    }
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
