#include <gtest/gtest.h>

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <utility>

class Vector {
  private:
    std::size_t capacity_;
    std::size_t size_;
    int *data_;

    void resize(std::size_t new_capacity) {
        // if (new_capacity < size_)

        int *new_data = new int[new_capacity]{};

        for (std::size_t i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }

        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }

  public:
    Vector() : capacity_(0), size_(0), data_(nullptr) {}

    Vector(std::initializer_list<int> list) : capacity_(std::size(list)), size_(std::size(list)) {
        data_ = capacity_ ? new int[capacity_]{} : nullptr;
        std::ranges::copy(list, data_);
    }

    Vector(const Vector &other) : capacity_(other.capacity_), size_(other.size_) {
        data_ = capacity_ ? new int[capacity_]{} : nullptr;
        std::ranges::copy(other.data_, other.data_ + other.size_, data_);
    }

    Vector(Vector &&other)
        : capacity_(std::exchange(other.capacity_, 0)), size_(std::exchange(other.size_, 0)),
          data_(std::exchange(other.data_, nullptr)) {}

    ~Vector() { delete[] data_; }

    std::size_t capacity() const { return capacity_; }
    std::size_t size() const { return size_; }

    bool empty() const { return size_ == 0; }

    void push_back(int value) {
        if (size_ >= capacity_) {
            std::size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            resize(new_capacity);
        }

        data_[size_++] = value;
    }

    void clear() { size_ = 0; }

    int at() {

    }

    int &operator[](std::size_t index) {
        return data_[index];
    }

    const int &operator[](std::size_t index) const {
        return data_[index];
    }

    Vector &operator=(Vector other) {
        swap(other);
        return *this;
    }

    void swap(Vector &other) {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }
};

void swap(Vector &lhs, Vector &rhs) { lhs.swap(rhs); }

TEST(EnhancedVectorTest, DefaultConstructor) {
    Vector vec;
    EXPECT_TRUE(vec.empty());
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 0);
}

TEST(EnhancedVectorTest, InitializerListConstructor) {
    Vector vec = {1, 2, 3, 4, 5};
    EXPECT_FALSE(vec.empty());
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec.capacity(), 5);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[4], 5);
}

TEST(EnhancedVectorTest, CopyConstructor) {
    Vector original = {1, 2, 3};
    Vector copy = original;

    EXPECT_EQ(copy.size(), 3);
    EXPECT_EQ(copy.capacity(), 3);
    EXPECT_EQ(copy[0], 1);
    EXPECT_EQ(copy[2], 3);
}

TEST(EnhancedVectorTest, MoveConstructor) {
    Vector original = {1, 2, 3};
    Vector moved = std::move(original);

    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved.capacity(), 3);
    EXPECT_EQ(moved[0], 1);
    EXPECT_TRUE(original.empty());
    EXPECT_EQ(original.capacity(), 0);
}

TEST(EnhancedVectorTest, PushBackIncreasesSize) {
    Vector vec;
    vec.push_back(10);

    EXPECT_FALSE(vec.empty());
    EXPECT_EQ(vec.size(), 1);
    EXPECT_GE(vec.capacity(), 1);
    EXPECT_EQ(vec[0], 10);
}

TEST(EnhancedVectorTest, PushBackMultipleElements) {
    Vector vec;

    for (int i = 0; i < 10; ++i) {
        vec.push_back(i * 10);
    }

    EXPECT_EQ(vec.size(), 10);
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(vec[i], i * 10);
    }
}

TEST(EnhancedVectorTest, CapacityGrowth) {
    Vector vec;

    std::size_t previous_capacity = vec.capacity();
    for (int i = 0; i < 20; ++i) {
        vec.push_back(i);
        if (vec.capacity() != previous_capacity) {
            previous_capacity = vec.capacity();
        }
    }

    EXPECT_GE(vec.capacity(), 20);
    EXPECT_EQ(vec.size(), 20);
}

TEST(EnhancedVectorTest, ClearResetsSizeNotCapacity) {
    Vector vec = {1, 2, 3, 4, 5};
    std::size_t original_capacity = vec.capacity();

    vec.clear();

    EXPECT_TRUE(vec.empty());
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), original_capacity);
}

TEST(EnhancedVectorTest, PushBackAfterClear) {
    Vector vec = {1, 2, 3};
    vec.clear();

    vec.push_back(4);
    vec.push_back(5);

    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0], 4);
    EXPECT_EQ(vec[1], 5);
}

TEST(EnhancedVectorTest, OperatorAccess) {
    Vector vec = {10, 20, 30};

    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);
    EXPECT_EQ(vec[2], 30);

    vec[1] = 25;
    EXPECT_EQ(vec[1], 25);
}

TEST(EnhancedVectorTest, OperatorAccessOutOfRange) {
    Vector vec = {1};

    EXPECT_THROW(vec[1], std::out_of_range);
    EXPECT_THROW(vec[100], std::out_of_range);
}

TEST(EnhancedVectorTest, CopyAssignment) {
    Vector vec1 = {1, 2, 3};
    Vector vec2 = {4, 5};

    vec2 = vec1;

    EXPECT_EQ(vec2.size(), 3);
    EXPECT_EQ(vec2[0], 1);
    EXPECT_EQ(vec2[2], 3);
}

TEST(EnhancedVectorTest, MoveAssignment) {
    Vector vec1 = {1, 2, 3};
    Vector vec2;

    vec2 = std::move(vec1);

    EXPECT_EQ(vec2.size(), 3);
    EXPECT_EQ(vec2[0], 1);
    EXPECT_TRUE(vec1.empty());
}

TEST(EnhancedVectorTest, Swap) {
    Vector vec1 = {1, 2, 3};
    Vector vec2 = {4, 5};

    swap(vec1, vec2);

    EXPECT_EQ(vec1.size(), 2);
    EXPECT_EQ(vec2.size(), 3);
    EXPECT_EQ(vec1[0], 4);
    EXPECT_EQ(vec2[0], 1);
}

TEST(EnhancedVectorTest, LargeNumberOfElements) {
    Vector vec;

    for (int i = 0; i < 1000; ++i) {
        vec.push_back(i);
    }

    EXPECT_EQ(vec.size(), 1000);
    EXPECT_GE(vec.capacity(), 1000);

    for (int i = 0; i < 1000; ++i) {
        EXPECT_EQ(vec[i], i);
    }
}

TEST(EnhancedVectorTest, CapacityDoublingPattern) {
    Vector vec;

    std::vector<std::size_t> capacities;
    for (int i = 0; i < 20; ++i) {
        vec.push_back(i);
        capacities.push_back(vec.capacity());
    }

    for (std::size_t i = 1; i < capacities.size(); ++i) {
        if (capacities[i] != capacities[i - 1]) {
            EXPECT_GE(capacities[i], capacities[i - 1] * 2);
        }
    }
}

TEST(EnhancedVectorTest, EmptyInitializerList) {
    Vector vec = {};

    EXPECT_TRUE(vec.empty());
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 0);
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}