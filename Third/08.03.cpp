#include <climits>
#include <cmath>
#include <iostream>

int binary_log_int(int x) {
    unsigned int u = static_cast<unsigned int>(x);
    int pos = -1;
    while (u) {
        u >>= 1;
        ++pos;
    }
    return pos;
}

int binary_log_float(float x) {
    union {
        float f;
        unsigned int u;
    } converter;
    converter.f = x;
    unsigned int u = converter.u;

    unsigned int sign = u >> 31;
    unsigned int exp = (u >> 23) & 0xFF;
    unsigned int mantissa = u & 0x7FFFFF;

    if (sign) {
        return -1;
    }

    if (exp == 255) {
        if (mantissa == 0)
            return INT_MAX;
        return 0;
    }

    if (exp > 0) {
        return static_cast<int>(exp) - 127;
    }

    if (mantissa == 0) {
        return INT_MIN;
    }

    int pos = -1;
    unsigned int m = mantissa;
    while (m) {
        m >>= 1;
        ++pos;
    }
    return pos - 149;
}

int main(int argc, const char **argv) {
#define BIN_INT(X) std::cout << "INT " << static_cast<int>(X) << ": " << binary_log_int(X) << "\n";
#define BIN_FLOAT(X)                                                                               \
    std::cout << "FLOAT " << static_cast<float>(X) << ": " << binary_log_float(X) << "\n";

    BIN_INT(12345);
    BIN_INT(0);
    BIN_FLOAT(12345.0);
    BIN_FLOAT(-1);
    BIN_FLOAT(NAN);
    BIN_FLOAT(INFINITY);
    BIN_FLOAT(0);
}
