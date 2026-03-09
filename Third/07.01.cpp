#include <cmath>
#include <iostream>
#include <optional>
#include <utility>
#include <variant>

static inline bool isZero(double x) {
    const double epsilon = 1e-10;
    return std::abs(x) < epsilon;
}

using RootVariant = std::variant<std::monostate, double, std::pair<double, double>>;

std::optional<RootVariant> solve(double a, double b, double c) {
    if (isZero(a)) {
        if (isZero(b)) {
            if (isZero(c)) {
                return RootVariant{std::monostate{}};
            } else {
                return std::nullopt;
            }
        } else {
            double root = -c / b;
            return RootVariant{root};
        }
    } else {
        double discriminant = b * b - 4 * a * c;
        if (isZero(discriminant)) {
            double root = -b / (2 * a);
            return RootVariant{root};
        } else if (discriminant < 0) {
            return std::nullopt;
        } else {
            double root1 = (-b - std::sqrt(discriminant)) / (2 * a);
            double root2 = (-b + std::sqrt(discriminant)) / (2 * a);
            return RootVariant{std::make_pair(root1, root2)};
        }
    }
}

#ifndef GTEST

int main() {
    double a = NAN, b = NAN, c = NAN;
    std::cin >> a >> b >> c;

    auto result = solve(a, b, c);
    if (!result) {
        if (isZero(a) && isZero(b)) {
            std::cout << "No solution\n";
        } else {
            std::cout << "No real roots\n";
        }
    } else {
        if (std::holds_alternative<std::monostate>(*result)) {
            std::cout << "Solution: R\n";
        } else if (std::holds_alternative<double>(*result)) {
            std::cout << std::get<double>(*result) << "\n";
        } else {
            auto p = std::get<std::pair<double, double>>(*result);
            std::cout << p.first << " " << p.second << "\n";
        }
    }

    return 0;
}

#else

#include <gtest/gtest.h>

// Google Test cases
TEST(SolveTest, InfiniteSolutions) {
    auto result = solve(0, 0, 0);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE((std::holds_alternative<std::monostate>(*result)));
}

TEST(SolveTest, NoSolutionLinear) {
    auto result = solve(0, 0, 5);
    ASSERT_FALSE(result.has_value());
}

TEST(SolveTest, LinearOneRoot) {
    auto result = solve(0, 2, -4);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE((std::holds_alternative<double>(*result)));
    EXPECT_DOUBLE_EQ(2.0, std::get<double>(*result));
}

TEST(SolveTest, QuadraticDiscriminantZero) {
    auto result = solve(1, -4, 4); // (x-2)^2 = 0
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE((std::holds_alternative<double>(*result)));
    EXPECT_DOUBLE_EQ(2.0, std::get<double>(*result));
}

TEST(SolveTest, QuadraticNoRealRoots) {
    auto result = solve(1, 0, 1); // x^2 + 1 = 0
    ASSERT_FALSE(result.has_value());
}

TEST(SolveTest, QuadraticTwoRoots) {
    auto result = solve(1, -3, 2); // x^2 - 3x + 2 = 0 => roots 1 and 2
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE((std::holds_alternative<std::pair<double, double>>(*result)));
    auto roots = std::get<std::pair<double, double>>(*result);
    EXPECT_DOUBLE_EQ(1.0, roots.first);
    EXPECT_DOUBLE_EQ(2.0, roots.second);
}

TEST(SolveTest, QuadraticTwoRootsUnsorted) {
    auto result = solve(1, 0, -4); // x^2 - 4 = 0 => roots -2, 2
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE((std::holds_alternative<std::pair<double, double>>(*result)));
    auto roots = std::get<std::pair<double, double>>(*result);
    EXPECT_DOUBLE_EQ(-2.0, roots.first);
    EXPECT_DOUBLE_EQ(2.0, roots.second);
}

TEST(SolveTest, NearZeroCoefficients) {
    auto result = solve(1e-11, 2, -4);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE((std::holds_alternative<double>(*result)));
    EXPECT_DOUBLE_EQ(2.0, std::get<double>(*result));
}

TEST(SolveTest, NearZeroDiscriminant) {
    auto result = solve(1, -2, 1 - 1e-12);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE((std::holds_alternative<double>(*result)));
    EXPECT_NEAR(1.0, std::get<double>(*result), 1e-6);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#endif // GTEST
