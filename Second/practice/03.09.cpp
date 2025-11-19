#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
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

    // Prefix increment
    IPv4 &operator++() {
        for (int i = 3; i >= 0; --i) {
            if (components[i] < 255) {
                ++components[i];
                break;
            } else {
                components[i] = 0;
                // Continue to next component (carry over)
            }
        }
        return *this;
    }

    // Postfix increment
    IPv4 operator++(int) {
        IPv4 temp = *this;
        ++(*this);
        return temp;
    }

    // Prefix decrement
    IPv4 &operator--() {
        for (int i = 3; i >= 0; --i) {
            if (components[i] > 0) {
                --components[i];
                break;
            } else {
                components[i] = 255;
                // Continue to next component (borrow)
            }
        }
        return *this;
    }

    // Postfix decrement
    IPv4 operator--(int) {
        IPv4 temp = *this;
        --(*this);
        return temp;
    }

    // Comparison operators
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

    // Input operator
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

    // Output operator
    friend std::ostream &operator<<(std::ostream &os, const IPv4 &ip) {
        os << static_cast<int>(ip.components[0]) << '.' << static_cast<int>(ip.components[1]) << '.'
           << static_cast<int>(ip.components[2]) << '.' << static_cast<int>(ip.components[3]);
        return os;
    }
};

int main() {
    // Test basic construction and output
    IPv4 ip1(192, 168, 1, 1);
    IPv4 ip2(10, 0, 0, 1);
    IPv4 ip3(255, 255, 255, 255);

    std::cout << "IP1: " << ip1 << "\n";
    std::cout << "IP2: " << ip2 << "\n";
    std::cout << "IP3: " << ip3 << "\n";

    // Test increment operators
    std::cout << "\nTesting increment:\n";
    IPv4 ip4(192, 168, 1, 254);
    std::cout << "Original: " << ip4 << "\n";
    std::cout << "Prefix++: " << ++ip4 << "\n";
    std::cout << "Postfix++: " << ip4++ << "\n";
    std::cout << "After postfix++: " << ip4 << "\n";

    // Test decrement operators
    std::cout << "\nTesting decrement:\n";
    IPv4 ip5(192, 168, 2, 1);
    std::cout << "Original: " << ip5 << "\n";
    std::cout << "Prefix--: " << --ip5 << "\n";
    std::cout << "Postfix--: " << ip5-- << "\n";
    std::cout << "After postfix--: " << ip5 << "\n";

    // Test boundary conditions
    std::cout << "\nTesting boundaries:\n";
    IPv4 max_ip(255, 255, 255, 255);
    IPv4 min_ip(0, 0, 0, 0);

    ++max_ip; // Should wrap around to 0.0.0.0
    std::cout << "After incrementing max: " << max_ip << "\n";

    --min_ip; // Should wrap around to 255.255.255.255
    std::cout << "After decrementing min: " << min_ip << "\n";

    // Test comparison operators
    std::cout << "\nTesting comparisons:\n";
    IPv4 a(192, 168, 1, 1);
    IPv4 b(192, 168, 1, 2);
    IPv4 c(192, 168, 1, 1);

    std::cout << a << " == " << c << ": " << (a == c) << "\n";
    std::cout << a << " != " << b << ": " << (a != b) << "\n";
    std::cout << a << " < " << b << ": " << (a < b) << "\n";
    std::cout << b << " > " << a << ": " << (b > a) << "\n";

    assert(a == c);
    assert(a != b);
    assert(a < b);
    assert(b > a);

    // Test input operator
    std::cout << "\nTesting input:\n";
    std::stringstream input1("10.20.30.40");
    std::stringstream input2("300.400.500.600"); // Invalid
    std::stringstream input3("1.2.3");           // Invalid

    IPv4 test_ip;
    input1 >> test_ip;
    std::cout << "Parsed from '10.20.30.40': " << test_ip << "\n";
    assert(test_ip == IPv4(10, 20, 30, 40));

    // Test invalid input
    if (!(input2 >> test_ip)) {
        std::cout << "Correctly rejected invalid IP: 300.400.500.600\n";
    }

    if (!(input3 >> test_ip)) {
        std::cout << "Correctly rejected incomplete IP: 1.2.3\n";
    }

    std::cout << "\nAll tests passed!\n";
    return 0;
}
