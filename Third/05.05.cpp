#include <cmath>
#include <compare>
#include <gtest/gtest.h>
#include <istream>
#include <numeric>
#include <ostream>
#include <sstream>
#include <stdexcept>

template <typename Derived> struct addable {
    friend Derived operator+(const Derived &lhs, const Derived &rhs) {
        Derived result(lhs);
        result += rhs;
        return result;
    }
};

template <typename Derived> struct subtractable {
    friend Derived operator-(const Derived &lhs, const Derived &rhs) {
        Derived result(lhs);
        result -= rhs;
        return result;
    }
};

template <typename Derived> struct multipliable {
    friend Derived operator*(const Derived &lhs, const Derived &rhs) {
        Derived result(lhs);
        result *= rhs;
        return result;
    }
};

template <typename Derived> struct dividable {
    friend Derived operator/(const Derived &lhs, const Derived &rhs) {
        Derived result(lhs);
        result /= rhs;
        return result;
    }
};

template <typename Derived> struct incrementable {
    friend Derived operator++(Derived &obj, int) {
        Derived tmp(obj);
        ++obj;
        return tmp;
    }
};

template <typename Derived> struct decrementable {
    friend Derived operator--(Derived &obj, int) {
        Derived tmp(obj);
        --obj;
        return tmp;
    }
};

template <typename T>
class Rational : public addable<Rational<T>>,
                 public subtractable<Rational<T>>,
                 public multipliable<Rational<T>>,
                 public dividable<Rational<T>>,
                 public incrementable<Rational<T>>,
                 public decrementable<Rational<T>> {
  private:
    T nominator = 0;
    T denominator = 1;

    void reduce() {
        if (denominator < 0) {
            nominator = -nominator;
            denominator = -denominator;
        }
        auto gcd = std::gcd(nominator, denominator);
        nominator /= gcd;
        denominator /= gcd;
    }

  public:
    Rational(T num = 0, T den = 1) : nominator(num), denominator(den) {
        if (den == 0)
            throw std::invalid_argument("denominator cannot be zero");
        reduce();
    }

    explicit operator double() const { return 1.0 * nominator / denominator; }

    Rational &operator+=(const Rational &other) {
        auto lcm = std::lcm(denominator, other.denominator);
        nominator = nominator * (lcm / denominator) + other.nominator * (lcm / other.denominator);
        denominator = lcm;
        reduce();
        return *this;
    }

    Rational &operator-=(const Rational &other) {
        return *this += Rational(other.nominator * -1, other.denominator);
    }

    Rational &operator*=(const Rational &other) {
        nominator *= other.nominator;
        denominator *= other.denominator;
        reduce();
        return *this;
    }

    Rational &operator/=(const Rational &other) {
        return *this *= Rational(other.denominator, other.nominator);
    }

    Rational &operator++() {
        *this += 1;
        return *this;
    }

    Rational &operator--() {
        *this -= 1;
        return *this;
    }

    friend std::strong_ordering operator<=>(const Rational &lhs, const Rational &rhs) {
        return lhs.nominator * rhs.denominator <=> rhs.nominator * lhs.denominator;
    }

    friend bool operator==(const Rational &lhs, const Rational &rhs) {
        return (lhs <=> rhs) == std::strong_ordering::equal;
    }

    friend std::istream &operator>>(std::istream &stream, Rational &rational) {
        T num, den;
        char slash;
        stream >> num;
        if (!stream)
            return stream;
        stream >> std::ws;
        stream >> slash;
        if (slash != '/') {
            stream.setstate(std::ios::failbit);
            return stream;
        }
        stream >> den;
        if (stream) {
            rational = Rational(num, den);
        }
        return stream;
    }

    friend std::ostream &operator<<(std::ostream &stream, const Rational &rational) {
        return stream << rational.nominator << '/' << rational.denominator;
    }
};

bool equal(double x, double y, double epsilon = 1e-6) { return std::abs(x - y) < epsilon; }

TEST(RationalTest, DefaultConstructor) {
    Rational<int> r;
    EXPECT_EQ(r, Rational(0, 1));
}

TEST(RationalTest, ParameterizedConstructor) {
    Rational r(1, 2);
    EXPECT_EQ(r, Rational(1, 2));
}

TEST(RationalTest, ConstructorReduction) {
    Rational r(2, 4);
    EXPECT_EQ(r, Rational(1, 2));
}

TEST(RationalTest, ConstructorNegativeDenominator) {
    Rational r(1, -2);
    EXPECT_EQ(r, Rational(-1, 2));
}

TEST(RationalTest, DoubleConversion) {
    Rational r(1, 2);
    EXPECT_TRUE(equal(static_cast<double>(r), 0.5));
}

TEST(RationalTest, AdditionAssignment) {
    Rational r(1, 2);
    r += Rational(1, 3);
    EXPECT_EQ(r, Rational(5, 6));
}

TEST(RationalTest, SubtractionAssignment) {
    Rational r(1, 2);
    r -= Rational(1, 3);
    EXPECT_EQ(r, Rational(1, 6));
}

TEST(RationalTest, MultiplicationAssignment) {
    Rational r(1, 2);
    r *= Rational(2, 3);
    EXPECT_EQ(r, Rational(1, 3));
}

TEST(RationalTest, DivisionAssignment) {
    Rational r(1, 2);
    r /= Rational(2, 3);
    EXPECT_EQ(r, Rational(3, 4));
}

TEST(RationalTest, PrefixIncrement) {
    Rational r(1, 2);
    Rational result = ++r;
    EXPECT_EQ(r, Rational(3, 2));
    EXPECT_EQ(result, Rational(3, 2));
}

TEST(RationalTest, PostfixIncrement) {
    Rational r(1, 2);
    Rational result = r++;
    EXPECT_EQ(r, Rational(3, 2));
    EXPECT_EQ(result, Rational(1, 2));
}

TEST(RationalTest, PrefixDecrement) {
    Rational r(3, 2);
    Rational result = --r;
    EXPECT_EQ(r, Rational(1, 2));
    EXPECT_EQ(result, Rational(1, 2));
}

TEST(RationalTest, PostfixDecrement) {
    Rational r(3, 2);
    Rational result = r--;
    EXPECT_EQ(r, Rational(1, 2));
    EXPECT_EQ(result, Rational(3, 2));
}

TEST(RationalTest, AdditionOperator) {
    Rational r1(1, 2);
    Rational r2(1, 3);
    Rational result = r1 + r2;
    EXPECT_EQ(result, Rational(5, 6));
}

TEST(RationalTest, SubtractionOperator) {
    Rational r1(1, 2);
    Rational r2(1, 3);
    Rational result = r1 - r2;
    EXPECT_EQ(result, Rational(1, 6));
}

TEST(RationalTest, MultiplicationOperator) {
    Rational r1(1, 2);
    Rational r2(2, 3);
    Rational result = r1 * r2;
    EXPECT_EQ(result, Rational(1, 3));
}

TEST(RationalTest, DivisionOperator) {
    Rational r1(1, 2);
    Rational r2(2, 3);
    Rational result = r1 / r2;
    EXPECT_EQ(result, Rational(3, 4));
}

TEST(RationalTest, ThreeWayComparisonEqual) {
    Rational r1(1, 2);
    Rational r2(2, 4);
    EXPECT_EQ(r1 <=> r2, std::strong_ordering::equal);
}

TEST(RationalTest, ThreeWayComparisonLess) {
    Rational r1(1, 3);
    Rational r2(1, 2);
    EXPECT_EQ(r1 <=> r2, std::strong_ordering::less);
}

TEST(RationalTest, ThreeWayComparisonGreater) {
    Rational r1(2, 3);
    Rational r2(1, 2);
    EXPECT_EQ(r1 <=> r2, std::strong_ordering::greater);
}

TEST(RationalTest, EqualityOperator) {
    Rational r1(1, 2);
    Rational r2(2, 4);
    Rational r3(1, 3);

    EXPECT_TRUE(r1 == r2);
    EXPECT_FALSE(r1 == r3);
}

TEST(RationalTest, InequalityOperator) {
    Rational r1(1, 2);
    Rational r2(1, 3);

    EXPECT_TRUE(r1 != r2);
    EXPECT_FALSE(r1 != Rational(2, 4));
}

TEST(RationalTest, LessThanOperator) {
    Rational r1(1, 3);
    Rational r2(1, 2);

    EXPECT_TRUE(r1 < r2);
    EXPECT_FALSE(r2 < r1);
}

TEST(RationalTest, GreaterThanOperator) {
    Rational r1(2, 3);
    Rational r2(1, 2);

    EXPECT_TRUE(r1 > r2);
    EXPECT_FALSE(r2 > r1);
}

TEST(RationalTest, LessThanOrEqualOperator) {
    Rational r1(1, 3);
    Rational r2(1, 2);
    Rational r3(2, 4);

    EXPECT_TRUE(r1 <= r2);
    EXPECT_TRUE(r2 <= r3);
    EXPECT_FALSE(r2 <= r1);
}

TEST(RationalTest, GreaterThanOrEqualOperator) {
    Rational r1(2, 3);
    Rational r2(1, 2);
    Rational r3(2, 4);

    EXPECT_TRUE(r1 >= r2);
    EXPECT_TRUE(r2 >= r3);
    EXPECT_FALSE(r2 >= r1);
}

TEST(RationalTest, InputOperator) {
    std::stringstream ss("3/4");
    Rational<int> r;
    ss >> r;
    EXPECT_EQ(r, Rational(3, 4));
    EXPECT_FALSE(ss.fail());
}

TEST(RationalTest, OutputOperator) {
    Rational r(3, 4);
    std::stringstream ss;
    ss << r;
    EXPECT_EQ(ss.str(), "3/4");
}

TEST(RationalTest, InputOutputConsistency) {
    Rational original(5, 8);
    std::stringstream ss;
    ss << original;

    Rational<int> parsed;
    ss >> parsed;

    EXPECT_EQ(original, parsed);
}

TEST(RationalTest, MixedOperations) {
    Rational r1(1, 2);
    Rational r2(1, 3);
    Rational r3(2, 5);

    Rational result = (r1 + r2) * r3 - r1 / r2;
    EXPECT_EQ(result, Rational(-7, 6));
}

TEST(RationalTest, IntegerOperations) {
    Rational r(1, 2);

    EXPECT_EQ(r + 1, Rational(3, 2));
    EXPECT_EQ(1 + r, Rational(3, 2));
    EXPECT_EQ(r - 1, Rational(-1, 2));
    EXPECT_EQ(1 - r, Rational(1, 2));
    EXPECT_EQ(r * 2, Rational(1, 1));
    EXPECT_EQ(2 * r, Rational(1, 1));
    EXPECT_EQ(r / 2, Rational(1, 4));
    EXPECT_EQ(2 / r, Rational(4, 1));
}

TEST(RationalTest, ZeroHandling) {
    Rational zero(0, 1);
    Rational r(1, 2);

    EXPECT_EQ(zero + r, r);
    EXPECT_EQ(r + zero, r);
    EXPECT_EQ(zero * r, zero);
    EXPECT_EQ(r * zero, zero);
    EXPECT_EQ(zero / r, zero);
}

TEST(RationalTest, LargeNumbers) {
    Rational r1(1000, 1);
    Rational r2(1, 1000);

    EXPECT_EQ(r1 * r2, Rational(1, 1));
    EXPECT_EQ(r1 / r2, Rational(1000000, 1));
}

TEST(RationalTest, ExpressionRewriting) {
    Rational a(1, 2);
    Rational b(1, 3);
    Rational c(2, 4);

    EXPECT_TRUE(a == c);
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a != c);
    EXPECT_TRUE(b < a);
    EXPECT_FALSE(a < b);
    EXPECT_TRUE(a > b);
    EXPECT_FALSE(b > a);
    EXPECT_TRUE(b <= a);
    EXPECT_TRUE(a <= c);
    EXPECT_FALSE(a <= b);
    EXPECT_TRUE(a >= c);
    EXPECT_TRUE(a >= b);
    EXPECT_FALSE(b >= a);
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
