#include <cassert>
#include <compare>
#include <iostream>
#include <numeric>
#include <sstream>
#if 0
class Rational {
  private:
    int numerator;
    int denominator;

    void normalize() {
        if (denominator == 0) {
            throw std::invalid_argument("Denominator cannot be zero");
        }

        if (denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }

        int gcd_val = std::gcd(numerator, denominator);
        numerator /= gcd_val;
        denominator /= gcd_val;
    }

  public:
    Rational(int num = 0, int den = 1) : numerator(num), denominator(den) { normalize(); }

    // Three-way comparison operator (spaceship)
    friend std::strong_ordering operator<=>(const Rational &lhs, const Rational &rhs) {
        // Compare cross products to avoid floating point
        long long left = static_cast<long long>(lhs.numerator) * rhs.denominator;
        long long right = static_cast<long long>(rhs.numerator) * lhs.denominator;

        if (left < right)
            return std::strong_ordering::less;
        if (left > right)
            return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }

    // Equality operator (can be generated from <=> in C++20, but we implement explicitly)
    friend bool operator==(const Rational &lhs, const Rational &rhs) {
        return lhs.numerator == rhs.numerator && lhs.denominator == rhs.denominator;
    }

    // Other operators can use rewrites with <=> and ==

    // Arithmetic operators
    friend Rational operator+(const Rational &lhs, const Rational &rhs) {
        return Rational(lhs.numerator * rhs.denominator + rhs.numerator * lhs.denominator,
                        lhs.denominator * rhs.denominator);
    }

    friend Rational operator-(const Rational &lhs, const Rational &rhs) {
        return Rational(lhs.numerator * rhs.denominator - rhs.numerator * lhs.denominator,
                        lhs.denominator * rhs.denominator);
    }

    friend Rational operator*(const Rational &lhs, const Rational &rhs) {
        return Rational(lhs.numerator * rhs.numerator, lhs.denominator * rhs.denominator);
    }

    friend Rational operator/(const Rational &lhs, const Rational &rhs) {
        return Rational(lhs.numerator * rhs.denominator, lhs.denominator * rhs.numerator);
    }

    // Input/output operators
    friend std::ostream &operator<<(std::ostream &os, const Rational &r) {
        os << r.numerator;
        if (r.denominator != 1) {
            os << '/' << r.denominator;
        }
        return os;
    }

    friend std::istream &operator>>(std::istream &is, Rational &r) {
        int num, den = 1;
        char slash = 0;

        is >> num;
        if (is.peek() == '/') {
            is >> slash >> den;
        }

        if (is) {
            r = Rational(num, den);
        }

        return is;
    }

    // Getters for testing
    int get_numerator() const { return numerator; }
    int get_denominator() const { return denominator; }
};

int main() {
    // Test basic construction and normalization
    Rational r1(2, 4);
    Rational r2(3, 6);
    Rational r3(-2, 4);
    Rational r4(2, -4);

    std::cout << "r1(2,4) = " << r1 << "\n";
    std::cout << "r2(3,6) = " << r2 << "\n";
    std::cout << "r3(-2,4) = " << r3 << "\n";
    std::cout << "r4(2,-4) = " << r4 << "\n";

    assert(r1.get_numerator() == 1 && r1.get_denominator() == 2);
    assert(r2.get_numerator() == 1 && r2.get_denominator() == 2);
    assert(r3.get_numerator() == -1 && r3.get_denominator() == 2);
    assert(r4.get_numerator() == -1 && r4.get_denominator() == 2);

    // Test three-way comparison
    std::cout << "\nTesting three-way comparison:\n";
    Rational a(1, 2);
    Rational b(1, 3);
    Rational c(2, 4);
    Rational d(3, 2);

    std::cout << a << " <=> " << b << ": ";
    auto result1 = a <=> b;
    if (result1 < 0)
        std::cout << "less\n";
    else if (result1 > 0)
        std::cout << "greater\n";
    else
        std::cout << "equal\n";

    std::cout << a << " <=> " << c << ": ";
    auto result2 = a <=> c;
    if (result2 < 0)
        std::cout << "less\n";
    else if (result2 > 0)
        std::cout << "greater\n";
    else
        std::cout << "equal\n";

    std::cout << d << " <=> " << a << ": ";
    auto result3 = d <=> a;
    if (result3 < 0)
        std::cout << "less\n";
    else if (result3 > 0)
        std::cout << "greater\n";
    else
        std::cout << "equal\n";

    assert((a <=> b) > 0);
    assert((a <=> c) == 0);
    assert((d <=> a) > 0);

    // Test equality operator
    std::cout << "\nTesting equality:\n";
    std::cout << a << " == " << c << ": " << (a == c) << "\n";
    std::cout << a << " == " << b << ": " << (a == b) << "\n";

    assert(a == c);
    assert(!(a == b));

    // Test rewritten comparison expressions
    std::cout << "\nTesting rewritten comparisons:\n";
    std::cout << a << " < " << b << ": " << (a < b) << "\n";
    std::cout << a << " > " << b << ": " << (a > b) << "\n";
    std::cout << a << " <= " << c << ": " << (a <= c) << "\n";
    std::cout << a << " >= " << c << ": " << (a >= c) << "\n";
    std::cout << a << " != " << b << ": " << (a != b) << "\n";

    assert(!(a < b));
    assert(a > b);
    assert(a <= c);
    assert(a >= c);
    assert(a != b);

    // Test arithmetic operations
    std::cout << "\nTesting arithmetic:\n";
    Rational x(1, 2);
    Rational y(1, 3);

    std::cout << x << " + " << y << " = " << (x + y) << "\n";
    std::cout << x << " - " << y << " = " << (x - y) << "\n";
    std::cout << x << " * " << y << " = " << (x * y) << "\n";
    std::cout << x << " / " << y << " = " << (x / y) << "\n";

    assert((x + y) == Rational(5, 6));
    assert((x - y) == Rational(1, 6));
    assert((x * y) == Rational(1, 6));
    assert((x / y) == Rational(3, 2));

    // Test input/output
    std::cout << "\nTesting I/O:\n";
    std::stringstream ss1("3/4");
    std::stringstream ss2("5");
    std::stringstream ss3("-2/3");

    Rational in1, in2, in3;
    ss1 >> in1;
    ss2 >> in2;
    ss3 >> in3;

    std::cout << "Read from '3/4': " << in1 << "\n";
    std::cout << "Read from '5': " << in2 << "\n";
    std::cout << "Read from '-2/3': " << in3 << "\n";

    assert(in1 == Rational(3, 4));
    assert(in2 == Rational(5, 1));
    assert(in3 == Rational(-2, 3));

    std::cout << "\nAll tests passed!\n";
    std::cout << "Successfully used three-way comparison with expression rewriting.\n";

    return 0;
}
#endif
