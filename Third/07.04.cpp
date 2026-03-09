#include <algorithm>
#include <cassert>
#include <cstddef>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

void order(std::vector<int> &vector, std::size_t left, std::size_t right) {
    for (auto i = left + 1; i < right; ++i) {
        for (auto j = i; j > left; --j) {
            if (vector[j - 1] > vector[j]) {
                std::swap(vector[j], vector[j - 1]);
            }
        }
    }
}

// Hoare's partition scheme with median-of-three pivot
std::size_t hoare_partition(std::vector<int> &vector, std::size_t left, std::size_t right) {
    // Median of three: first, middle, last elements
    // std::midpoint()
    std::size_t mid = left + (right - left) / 2;

    // Sort the three elements and use median as pivot
    if (vector[left] > vector[mid]) {
        std::swap(vector[left], vector[mid]);
    }
    if (vector[left] > vector[right - 1]) {
        std::swap(vector[left], vector[right - 1]);
    }
    if (vector[mid] > vector[right - 1]) {
        std::swap(vector[mid], vector[right - 1]);
    }

    // Place pivot at position right-2 (one before last)
    std::swap(vector[mid], vector[right - 2]);
    int pivot = vector[right - 2];

    // Hoare's partition
    std::size_t i = left;
    std::size_t j = right - 2;

    while (true) {
        // Find element on left that should be on right
        while (vector[++i] < pivot) {
        }

        // Find element on right that should be on left
        while (vector[--j] > pivot) {
        }

        if (i < j) {
            std::swap(vector[i], vector[j]);
        } else {
            break;
        }
    }

    // Restore pivot to final position
    std::swap(vector[i], vector[right - 2]);
    return i;
}

void split(std::vector<int> &vector, std::size_t left, std::size_t right) {
    if (right - left > 16) {
        // Use quick sort for larger partitions
        auto pivot_index = hoare_partition(vector, left, right);

        split(vector, left, pivot_index);
        split(vector, pivot_index + 1, right);
    } else {
        // Use insertion sort for small partitions
        order(vector, left, right);
    }
}

void sort(std::vector<int> &vector) {
    if (vector.size() > 1) {
        split(vector, 0, std::size(vector));
    }
}

TEST(Test10, TestDesc) {
    static const int size = 1000;

    std::vector<int> vector(size, 0);
    for (auto i = 0; i < size; ++i) {
        vector[i] = size - i;
    }
    sort(vector);

    ASSERT_TRUE(std::ranges::is_sorted(vector));
}

TEST(Test10, TestAsc) {
    static const int size = 1000;

    std::vector<int> vector(size, 0);
    for (auto i = 0; i < size; ++i) {
        vector[i] = i;
    }
    sort(vector);

    ASSERT_TRUE(std::ranges::is_sorted(vector));
}

TEST(Test10, TestRand) {
    std::vector<int> random_vector = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    sort(random_vector);

    ASSERT_TRUE(std::ranges::is_sorted(random_vector));
}

TEST(Test10, TestEmpty) {
    std::vector<int> empty_vector;
    sort(empty_vector);
    ASSERT_TRUE(empty_vector.empty());
}

TEST(Test10, TestSingle) {
    std::vector<int> single_vector = {42};
    sort(single_vector);
    ASSERT_EQ(single_vector.size(), 1);
    ASSERT_EQ(single_vector[0], 42);
}

TEST(Test10, TestDupl) {
    std::vector<int> duplicates = {5, 2, 5, 1, 5, 2, 1, 1};
    sort(duplicates);
    ASSERT_TRUE(std::ranges::is_sorted(duplicates));
}

int main() {
    // Time Complexity:
    // - Best/Average case: O(n log n) - good pivot selection from median-of-three
    // - Worst case: O(n²) - though median-of-three reduces probability significantly
    // - Hybrid approach: insertion sort for small partitions (n ≤ 16) is O(n²) but faster for small
    // n
    // - Space complexity: O(log n) due to recursion depth

    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
