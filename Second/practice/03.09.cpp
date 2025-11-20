#include <array>
#include <cstdint>
#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>
#include <string>

class IPv4 {
  private:
    std::array<std::uint8_t, 4> components;

    void validate_component(std::uint8_t value) const {
        if (value > 255) {
            throw std::out_of_range("IP component out of range");
        }
    }

  public:
    IPv4() : components{0, 0, 0, 0} {}

    IPv4(std::uint8_t a, std::uint8_t b, std::uint8_t c, std::uint8_t d) : components{a, b, c, d} {
        validate_component(a);
        validate_component(b);
        validate_component(c);
        validate_component(d);
    }

    IPv4 &operator++() {
        for (int i = 3; i >= 0; --i) {
            if (components[i] < 255) {
                ++components[i];
                break;
            } else {
                components[i] = 0;
            }
        }
        return *this;
    }

    IPv4 operator++(int) {
        IPv4 temp = *this;
        ++(*this);
        return temp;
    }

    IPv4 &operator--() {
        for (int i = 3; i >= 0; --i) {
            if (components[i] > 0) {
                --components[i];
                break;
            } else {
                components[i] = 255;
            }
        }
        return *this;
    }

    IPv4 operator--(int) {
        IPv4 temp = *this;
        --(*this);
        return temp;
    }

    friend bool operator==(const IPv4 &lhs, const IPv4 &rhs) {
        return lhs.components == rhs.components;
    }

    friend bool operator!=(const IPv4 &lhs, const IPv4 &rhs) { return !(lhs == rhs); }

    friend bool operator<(const IPv4 &lhs, const IPv4 &rhs) {
        return lhs.components < rhs.components;
    }

    friend bool operator>(const IPv4 &lhs, const IPv4 &rhs) { return rhs < lhs; }

    friend bool operator<=(const IPv4 &lhs, const IPv4 &rhs) { return !(rhs < lhs); }

    friend bool operator>=(const IPv4 &lhs, const IPv4 &rhs) { return !(lhs < rhs); }

    friend std::istream &operator>>(std::istream &is, IPv4 &ip) {
        std::string input;
        is >> input;

        std::stringstream ss(input);
        std::string token;
        std::array<std::uint8_t, 4> new_components;
        int index = 0;

        while (std::getline(ss, token, '.') && index < 4) {
            try {
                int value = std::stoi(token);
                if (value < 0 || value > 255) {
                    is.setstate(std::ios::failbit);
                    return is;
                }
                new_components[index++] = static_cast<std::uint8_t>(value);
            } catch (const std::exception &) {
                is.setstate(std::ios::failbit);
                return is;
            }
        }

        if (index == 4) {
            ip.components = new_components;
        } else {
            is.setstate(std::ios::failbit);
        }

        return is;
    }

    friend std::ostream &operator<<(std::ostream &os, const IPv4 &ip) {
        os << static_cast<int>(ip.components[0]) << '.' << static_cast<int>(ip.components[1]) << '.'
           << static_cast<int>(ip.components[2]) << '.' << static_cast<int>(ip.components[3]);
        return os;
    }
};

TEST(IPv4Test, DefaultConstructor) {
    IPv4 ip;
    std::stringstream ss;
    ss << ip;
    EXPECT_EQ(ss.str(), "0.0.0.0");
}

TEST(IPv4Test, ParameterizedConstructor) {
    IPv4 ip(192, 168, 1, 1);
    std::stringstream ss;
    ss << ip;
    EXPECT_EQ(ss.str(), "192.168.1.1");
}

TEST(IPv4Test, ValidComponentRange) {
    EXPECT_NO_THROW(IPv4(0, 0, 0, 0));
    EXPECT_NO_THROW(IPv4(255, 255, 255, 255));
    EXPECT_NO_THROW(IPv4(127, 0, 0, 1));
}

TEST(IPv4Test, PrefixIncrement) {
    IPv4 ip(192, 168, 1, 254);
    ++ip;
    std::stringstream ss;
    ss << ip;
    EXPECT_EQ(ss.str(), "192.168.1.255");
}

TEST(IPv4Test, PrefixIncrementWrapAround) {
    IPv4 ip(192, 168, 1, 255);
    ++ip;
    std::stringstream ss;
    ss << ip;
    EXPECT_EQ(ss.str(), "192.168.2.0");
}

TEST(IPv4Test, PrefixIncrementFullWrap) {
    IPv4 ip(255, 255, 255, 255);
    ++ip;
    std::stringstream ss;
    ss << ip;
    EXPECT_EQ(ss.str(), "0.0.0.0");
}

TEST(IPv4Test, PostfixIncrement) {
    IPv4 ip(10, 0, 0, 1);
    IPv4 old = ip++;
    std::stringstream ss1, ss2;
    ss1 << old;
    ss2 << ip;
    EXPECT_EQ(ss1.str(), "10.0.0.1");
    EXPECT_EQ(ss2.str(), "10.0.0.2");
}

TEST(IPv4Test, PrefixDecrement) {
    IPv4 ip(192, 168, 2, 0);
    --ip;
    std::stringstream ss;
    ss << ip;
    EXPECT_EQ(ss.str(), "192.168.1.255");
}

TEST(IPv4Test, PrefixDecrementWrapAround) {
    IPv4 ip(192, 168, 0, 0);
    --ip;
    std::stringstream ss;
    ss << ip;
    EXPECT_EQ(ss.str(), "192.167.255.255");
}

TEST(IPv4Test, PrefixDecrementFullWrap) {
    IPv4 ip(0, 0, 0, 0);
    --ip;
    std::stringstream ss;
    ss << ip;
    EXPECT_EQ(ss.str(), "255.255.255.255");
}

TEST(IPv4Test, PostfixDecrement) {
    IPv4 ip(10, 0, 0, 5);
    IPv4 old = ip--;
    std::stringstream ss1, ss2;
    ss1 << old;
    ss2 << ip;
    EXPECT_EQ(ss1.str(), "10.0.0.5");
    EXPECT_EQ(ss2.str(), "10.0.0.4");
}

TEST(IPv4Test, EqualityOperator) {
    IPv4 ip1(192, 168, 1, 1);
    IPv4 ip2(192, 168, 1, 1);
    IPv4 ip3(10, 0, 0, 1);

    EXPECT_TRUE(ip1 == ip2);
    EXPECT_FALSE(ip1 == ip3);
}

TEST(IPv4Test, InequalityOperator) {
    IPv4 ip1(192, 168, 1, 1);
    IPv4 ip2(192, 168, 1, 2);

    EXPECT_TRUE(ip1 != ip2);
    EXPECT_FALSE(ip1 != ip1);
}

TEST(IPv4Test, LessThanOperator) {
    IPv4 ip1(192, 168, 1, 1);
    IPv4 ip2(192, 168, 1, 2);
    IPv4 ip3(192, 168, 2, 1);

    EXPECT_TRUE(ip1 < ip2);
    EXPECT_TRUE(ip1 < ip3);
    EXPECT_FALSE(ip2 < ip1);
}

TEST(IPv4Test, GreaterThanOperator) {
    IPv4 ip1(192, 168, 1, 2);
    IPv4 ip2(192, 168, 1, 1);

    EXPECT_TRUE(ip1 > ip2);
    EXPECT_FALSE(ip2 > ip1);
}

TEST(IPv4Test, LessThanOrEqualOperator) {
    IPv4 ip1(192, 168, 1, 1);
    IPv4 ip2(192, 168, 1, 1);
    IPv4 ip3(192, 168, 1, 2);

    EXPECT_TRUE(ip1 <= ip2);
    EXPECT_TRUE(ip1 <= ip3);
    EXPECT_FALSE(ip3 <= ip1);
}

TEST(IPv4Test, GreaterThanOrEqualOperator) {
    IPv4 ip1(192, 168, 1, 2);
    IPv4 ip2(192, 168, 1, 2);
    IPv4 ip3(192, 168, 1, 1);

    EXPECT_TRUE(ip1 >= ip2);
    EXPECT_TRUE(ip1 >= ip3);
    EXPECT_FALSE(ip3 >= ip1);
}

TEST(IPv4Test, OutputOperator) {
    IPv4 ip(127, 0, 0, 1);
    std::stringstream ss;
    ss << ip;
    EXPECT_EQ(ss.str(), "127.0.0.1");
}

TEST(IPv4Test, InputOperatorValid) {
    std::stringstream ss("192.168.1.100");
    IPv4 ip;
    ss >> ip;

    EXPECT_FALSE(ss.fail());
    std::stringstream output;
    output << ip;
    EXPECT_EQ(output.str(), "192.168.1.100");
}

TEST(IPv4Test, InputOperatorMultipleValid) {
    std::stringstream ss("10.0.0.1 172.16.0.1 192.168.1.1");
    IPv4 ip1, ip2, ip3;
    ss >> ip1 >> ip2 >> ip3;

    EXPECT_FALSE(ss.fail());
    EXPECT_EQ(ip1, IPv4(10, 0, 0, 1));
    EXPECT_EQ(ip2, IPv4(172, 16, 0, 1));
    EXPECT_EQ(ip3, IPv4(192, 168, 1, 1));
}

TEST(IPv4Test, InputOperatorInvalidFormat) {
    std::stringstream ss("192.168.1");
    IPv4 ip;
    ss >> ip;
    EXPECT_TRUE(ss.fail());
}

TEST(IPv4Test, InputOperatorInvalidCharacter) {
    std::stringstream ss("192.168.1.x");
    IPv4 ip;
    ss >> ip;
    EXPECT_TRUE(ss.fail());
}

TEST(IPv4Test, InputOperatorOutOfRange) {
    std::stringstream ss("192.168.1.300");
    IPv4 ip;
    ss >> ip;
    EXPECT_TRUE(ss.fail());
}

TEST(IPv4Test, InputOperatorNegative) {
    std::stringstream ss("192.168.1.-1");
    IPv4 ip;
    ss >> ip;
    EXPECT_TRUE(ss.fail());
}

TEST(IPv4Test, InputOutputConsistency) {
    IPv4 original(8, 8, 8, 8);
    std::stringstream ss;
    ss << original;

    IPv4 parsed;
    ss >> parsed;

    EXPECT_EQ(original, parsed);
}

TEST(IPv4Test, IncrementDecrementChain) {
    IPv4 ip(10, 0, 0, 254);
    ++ip;
    ++ip;
    --ip;

    std::stringstream ss;
    ss << ip;
    EXPECT_EQ(ss.str(), "10.0.0.255");
}

TEST(IPv4Test, BoundaryValues) {
    EXPECT_NO_THROW(IPv4(0, 0, 0, 0));
    EXPECT_NO_THROW(IPv4(255, 255, 255, 255));

    std::stringstream ss1, ss2;
    ss1 << IPv4(0, 0, 0, 0);
    ss2 << IPv4(255, 255, 255, 255);

    EXPECT_EQ(ss1.str(), "0.0.0.0");
    EXPECT_EQ(ss2.str(), "255.255.255.255");
}

TEST(IPv4Test, ComparisonConsistency) {
    IPv4 ip1(192, 168, 1, 1);
    IPv4 ip2(192, 168, 1, 2);
    IPv4 ip3(192, 168, 1, 1);

    EXPECT_TRUE(ip1 == ip3);
    EXPECT_TRUE(ip1 != ip2);
    EXPECT_TRUE(ip1 < ip2);
    EXPECT_TRUE(ip2 > ip1);
    EXPECT_TRUE(ip1 <= ip2);
    EXPECT_TRUE(ip1 <= ip3);
    EXPECT_TRUE(ip2 >= ip1);
    EXPECT_TRUE(ip1 >= ip3);
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}