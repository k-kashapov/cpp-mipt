#include <algorithm>
#include <benchmark/benchmark.h>
#include <cassert>
#include <cstddef>
#include <gtest/gtest.h>
#include <utility>
#include <vector>

template <typename T> void order(std::vector<T> &vector, std::size_t left, std::size_t right) {
    for (auto i = left + 1; i < right; ++i) {
        for (auto j = i; j > left; --j) {
            if (vector[j - 1] > vector[j]) {
                std::swap(vector[j], vector[j - 1]);
            }
        }
    }
}

template <typename T>
std::size_t hoare_partition(std::vector<T> &vector, std::size_t left, std::size_t right) {
    std::size_t mid = left + (right - left) / 2;
    if (vector[left] > vector[mid]) {
        std::swap(vector[left], vector[mid]);
    }
    if (vector[left] > vector[right - 1]) {
        std::swap(vector[left], vector[right - 1]);
    }
    if (vector[mid] > vector[right - 1]) {
        std::swap(vector[mid], vector[right - 1]);
    }
    std::swap(vector[mid], vector[right - 2]);
    T pivot = vector[right - 2];
    std::size_t i = left;
    std::size_t j = right - 2;
    while (true) {
        while (vector[++i] < pivot) {
        }
        while (vector[--j] > pivot) {
        }
        if (i < j) {
            std::swap(vector[i], vector[j]);
        } else {
            break;
        }
    }
    std::swap(vector[i], vector[right - 2]);
    return i;
}

template <typename T>
void split(std::vector<T> &vector, std::size_t left, std::size_t right, std::size_t threshold) {
    if (right - left > threshold) {
        auto pivot_index = hoare_partition(vector, left, right);
        split(vector, left, pivot_index, threshold);
        split(vector, pivot_index + 1, right, threshold);
    } else {
        order(vector, left, right);
    }
}

template <typename T> void sort(std::vector<T> &vector, std::size_t threshold = 16) {
    if (vector.size() > 1) {
        split(vector, 0, std::size(vector), threshold);
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

static void BM_SortThreshold(benchmark::State &state) {
    const std::size_t threshold = state.range(0);
    const std::size_t size = 10000;
    std::vector<double> initial(size);
    for (std::size_t i = 0; i < size; ++i) {
        initial[i] = static_cast<double>(size - i);
    }
    for (auto _ : state) {
        std::vector<double> vec = initial;
        sort(vec, threshold);
        benchmark::DoNotOptimize(vec.data());
    }
}

BENCHMARK(BM_SortThreshold)->Arg(4)->Arg(8)->Arg(16)->Arg(32)->Arg(64)->Arg(128);

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    int testResult = RUN_ALL_TESTS();
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return testResult;
}
