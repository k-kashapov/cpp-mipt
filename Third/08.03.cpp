#include <climits>
#include <cmath>
#include <iostream>
#include <gtest/gtest.h>

int binary_log_int(int x) {
    if (x <= 0) {
        return INT_MIN;
    }

    unsigned int u = static_cast<unsigned int>(x);
    int pos = -1;
    while (u) {
        u >>= 1;
        ++pos;
    }
    return pos;
}

int binary_log_float(float x) {
    if (x <= 0) {
        return INT_MIN;
    }

    union {
        float f;
        unsigned int u;
    } converter;
    converter.f = x;
    unsigned int u = converter.u;

    unsigned int sign = u >> 31;
    unsigned int exp = (u >> 23) & 0xFF;
    unsigned int mantissa = u & 0x7FFFFF;

    if (exp == 255) {
        if (mantissa == 0)
            return INT_MAX;
        return INT_MIN;
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

// WARINING: tests are generated using DeepSeek

// Helper to create float subnormal numbers
float make_subnormal(int mantissa_bits) {
    // Build a subnormal: exponent = 0, mantissa = mantissa_bits (23 bits max)
    uint32_t bits = mantissa_bits;  // mantissa only (bits 22..0)
    float f;
    uint32_t u = bits;  // exponent and sign are zero
    memcpy(&f, &u, sizeof(f));
    return f;
}

// ---------- Tests for binary_log_int ----------
TEST(BinaryLogIntTest, PositiveNumbers) {
    EXPECT_EQ(binary_log_int(1), 0);
    EXPECT_EQ(binary_log_int(2), 1);
    EXPECT_EQ(binary_log_int(3), 1);
    EXPECT_EQ(binary_log_int(4), 2);
    EXPECT_EQ(binary_log_int(7), 2);
    EXPECT_EQ(binary_log_int(8), 3);
    EXPECT_EQ(binary_log_int(12345), 13);  // because 2^13 = 8192, 2^14 = 16384
    EXPECT_EQ(binary_log_int(65535), 15);
    EXPECT_EQ(binary_log_int(65536), 16);
    EXPECT_EQ(binary_log_int(INT_MAX), 30); // INT_MAX = 2^31-1, highest bit is 30
}

TEST(BinaryLogIntTest, ZeroAndNegative) {
    EXPECT_EQ(binary_log_int(0), INT_MIN);
    EXPECT_EQ(binary_log_int(-1), INT_MIN);
    EXPECT_EQ(binary_log_int(-100), INT_MIN);
}

// ---------- Tests for binary_log_float ----------
TEST(BinaryLogFloatTest, NormalNumbers) {
    EXPECT_EQ(binary_log_float(1.0f), 0);
    EXPECT_EQ(binary_log_float(2.0f), 1);
    EXPECT_EQ(binary_log_float(3.0f), 1);
    EXPECT_EQ(binary_log_float(4.0f), 2);
    EXPECT_EQ(binary_log_float(7.999f), 2);   // still <8
    EXPECT_EQ(binary_log_float(8.0f), 3);
    EXPECT_EQ(binary_log_float(12345.0f), 13);
    EXPECT_EQ(binary_log_float(65535.0f), 15);
    EXPECT_EQ(binary_log_float(65536.0f), 16);
    EXPECT_EQ(binary_log_float(8388608.0f), 23); // 2^23-1
    EXPECT_EQ(binary_log_float(1.18e-38f), -126); // smallest normal
}

TEST(BinaryLogFloatTest, ZeroAndNegative) {
    EXPECT_EQ(binary_log_float(0.0f), INT_MIN);
    EXPECT_EQ(binary_log_float(-0.0f), INT_MIN);  // negative zero
    EXPECT_EQ(binary_log_float(-1.0f), INT_MIN);
    EXPECT_EQ(binary_log_float(-3.14f), INT_MIN);
}

TEST(BinaryLogFloatTest, InfinityAndNaN) {
    EXPECT_EQ(binary_log_float(INFINITY), INT_MAX);
    EXPECT_EQ(binary_log_float(-INFINITY), INT_MIN); // negative infinity is <=0
    EXPECT_EQ(binary_log_float(NAN), INT_MIN);
    EXPECT_EQ(binary_log_float(-NAN), INT_MIN);
}

TEST(BinaryLogFloatTest, SubnormalNumbers) {
    // Smallest positive subnormal: 2^-149 ≈ 1.4e-45
    float smallest = make_subnormal(1);   // mantissa = 1, exponent = 0
    EXPECT_EQ(binary_log_float(smallest), -149);

    // Next subnormal: 2^-148
    float next = make_subnormal(2);
    EXPECT_EQ(binary_log_float(next), -148);

    // Largest subnormal: mantissa = 0x7FFFFF, value ~ 2^-126 * (1 - 2^-23)
    float largest_sub = make_subnormal(0x7FFFFF);
    EXPECT_EQ(binary_log_float(largest_sub), -127); // because exponent = -126, plus fraction's highest bit position

    // A few in‑between
    EXPECT_EQ(binary_log_float(make_subnormal(0x1000)), -149 + 12); // 0x1000 has bit 12 set -> position = 12
}

TEST(BinaryLogFloatTest, Boundaries) {
    // Largest normal float: (2-2^-23) * 2^127
    float max_normal = 3.402823466e+38f;
    EXPECT_EQ(binary_log_float(max_normal), 127);

    // Smallest normal: 2^-126
    float min_normal = 1.175494351e-38f;
    EXPECT_EQ(binary_log_float(min_normal), -126);

    // Just below smallest normal (should be subnormal)
    float just_below = min_normal / 2.0f;
    EXPECT_EQ(binary_log_float(just_below), -127);
}

TEST(BinaryLogFloatTest, PowersOfTwo) {
    for (int e = -126; e <= 127; ++e) {
        float f = ldexpf(1.0f, e);  // 2^e
        EXPECT_EQ(binary_log_float(f), e);
    }
}

TEST(BinaryLogFloatTest, RandomValues) {
    // Test a few random numbers
    float vals[] = { 0.15625f, 3.14159f, 100.0f, 1.0e10f, 1.0e-10f };
    for (float v : vals) {
        int expected = static_cast<int>(floor(log2f(v)));
        EXPECT_EQ(binary_log_float(v), expected);
    }
}

int main(int argc, const char **argv) {
#define BIN_INT(X) std::cout << "INT " << static_cast<int>(X) << ": " << binary_log_int(X) << "\n";
#define BIN_FLOAT(X)                                                                               \
    std::cout << "FLOAT " << static_cast<float>(X) << ": " << binary_log_float(X) << "\n";

    BIN_INT(12345);
    BIN_INT(0);
    BIN_INT(-1);
    BIN_FLOAT(12345.0);
    BIN_FLOAT(-1);
    BIN_FLOAT(NAN);
    BIN_FLOAT(INFINITY);
    BIN_FLOAT(0);

    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
