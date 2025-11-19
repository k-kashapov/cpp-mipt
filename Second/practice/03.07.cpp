#include <algorithm>
#include <cassert>
#include <iostream>

template <typename T> class Vector {
  private:
    std::size_t capacity_;
    std::size_t size_;
    T *data_;

    void resize(std::size_t new_capacity) {
        T *new_data = new T[new_capacity];

        // Copy existing elements
        for (std::size_t i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }

        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }

  public:
    Vector() : capacity_(0), size_(0), data_(nullptr) {}

    ~Vector() { delete[] data_; }

    std::size_t capacity() const { return capacity_; }
    std::size_t size() const { return size_; }

    bool empty() const { return size_ == 0; }

    void push_back(const T &value) {
        if (size_ >= capacity_) {
            // Double the capacity (or start with 1 if empty)
            std::size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            resize(new_capacity);
        }

        data_[size_] = value;
        ++size_;
    }

    void clear() {
        size_ = 0;
        // Note: capacity remains the same, memory is not freed
    }

    T &operator[](std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    const T &operator[](std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    // Prevent copying for simplicity
    Vector(const Vector &) = delete;
    Vector &operator=(const Vector &) = delete;
};

int main() {
    // Test empty vector
    Vector<int> vec;
    assert(vec.empty());
    assert(vec.size() == 0);
    assert(vec.capacity() == 0);
    std::cout << "Empty vector test passed\n";

    // Test push_back with resizing
    std::cout << "Testing push_back with capacity growth:\n";
    for (int i = 0; i < 10; ++i) {
        vec.push_back(i * 10);
        std::cout << "Size: " << vec.size() << ", Capacity: " << vec.capacity()
                  << ", Last element: " << vec[vec.size() - 1] << "\n";

        // Verify elements are correctly stored
        for (std::size_t j = 0; j < vec.size(); ++j) {
            assert(vec[j] == static_cast<int>(j * 10));
        }
    }

    // Test clear
    vec.clear();
    assert(vec.empty());
    assert(vec.size() == 0);
    assert(vec.capacity() > 0); // Capacity should be preserved
    std::cout << "Clear test passed\n";

    // Test push_back after clear
    vec.push_back(100);
    vec.push_back(200);
    assert(vec.size() == 2);
    assert(vec[0] == 100);
    assert(vec[1] == 200);
    std::cout << "Push after clear test passed\n";

    // Test with strings
    Vector<std::string> str_vec;
    str_vec.push_back("Hello");
    str_vec.push_back("World");
    str_vec.push_back("Test");

    assert(str_vec.size() == 3);
    assert(str_vec[0] == "Hello");
    assert(str_vec[1] == "World");
    assert(str_vec[2] == "Test");
    std::cout << "String vector test passed\n";

    // Test capacity growth pattern
    Vector<int> growth_test;
    std::size_t previous_capacity = 0;
    for (int i = 0; i < 20; ++i) {
        growth_test.push_back(i);
        if (growth_test.capacity() != previous_capacity) {
            std::cout << "Capacity grew from " << previous_capacity << " to "
                      << growth_test.capacity() << "\n";
            previous_capacity = growth_test.capacity();
        }
    }

    std::cout << "All tests passed!\n";
    return 0;
}
