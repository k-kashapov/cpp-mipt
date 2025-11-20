#include <gtest/gtest.h>
#include <iostream>
#include <stdexcept>

class List {
  private:
    struct Node {
        int value;
        Node *next;

        Node(int val, Node *nxt = nullptr) : value(val), next(nxt) {}
    };

    Node *head = nullptr;
    Node *tail = nullptr;

  public:
    bool empty() const { return head == nullptr; }

    void show() const {
        Node *current = head;
        while (current != nullptr) {
            std::cout << current->value << " -> ";
            current = current->next;
        }
        std::cout << "\n";
    }

    void push_front(int value) {
        Node *new_node = new Node(value, head);
        head = new_node;
        if (tail == nullptr) {
            tail = new_node;
        }
    }

    void push_back(int value) {
        Node *new_node = new Node(value);
        if (empty()) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }

    void pop_front() {
        if (empty()) {
            throw std::runtime_error("Cannot pop from empty list");
        }

        Node *temp = head;
        head = head->next;
        if (head == nullptr) {
            tail = nullptr;
        }
        delete temp;
    }

    void pop_back() {
        if (empty()) {
            throw std::runtime_error("Cannot pop from empty list");
        }

        if (head == tail) {
            delete head;
            head = tail = nullptr;
            return;
        }

        Node *current = head;
        while (current->next != tail) {
            current = current->next;
        }

        delete tail;
        tail = current;
        tail->next = nullptr;
    }

    int get() const {
        if (empty()) {
            throw std::runtime_error("Cannot get from empty list");
        }

        Node *slow = head;
        Node *fast = head;

        while (fast->next != nullptr && fast->next->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }

        return slow->value;
    }

    ~List() {
        while (!empty()) {
            pop_front();
        }
    }
};

TEST(ListTest, DefaultConstructor) {
    List list;
    EXPECT_TRUE(list.empty());
}

TEST(ListTest, PushFrontOnEmptyList) {
    List list;
    list.push_front(42);
    EXPECT_FALSE(list.empty());
}

TEST(ListTest, PushBackOnEmptyList) {
    List list;
    list.push_back(42);
    EXPECT_FALSE(list.empty());
}

TEST(ListTest, PushFrontMultiple) {
    List list;
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);
    EXPECT_FALSE(list.empty());
}

TEST(ListTest, PushBackMultiple) {
    List list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    EXPECT_FALSE(list.empty());
}

TEST(ListTest, PopFrontOnEmptyList) {
    List list;
    EXPECT_THROW(list.pop_front(), std::runtime_error);
}

TEST(ListTest, PopBackOnEmptyList) {
    List list;
    EXPECT_THROW(list.pop_back(), std::runtime_error);
}

TEST(ListTest, PopFrontSingleElement) {
    List list;
    list.push_front(42);
    list.pop_front();
    EXPECT_TRUE(list.empty());
}

TEST(ListTest, PopBackSingleElement) {
    List list;
    list.push_back(42);
    list.pop_back();
    EXPECT_TRUE(list.empty());
}

TEST(ListTest, PopFrontMultiple) {
    List list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    list.pop_front();
    EXPECT_FALSE(list.empty());

    list.pop_front();
    EXPECT_FALSE(list.empty());

    list.pop_front();
    EXPECT_TRUE(list.empty());
}

TEST(ListTest, PopBackMultiple) {
    List list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    list.pop_back();
    EXPECT_FALSE(list.empty());

    list.pop_back();
    EXPECT_FALSE(list.empty());

    list.pop_back();
    EXPECT_TRUE(list.empty());
}

TEST(ListTest, GetOnEmptyList) {
    List list;
    EXPECT_THROW(list.get(), std::runtime_error);
}

TEST(ListTest, GetSingleElement) {
    List list;
    list.push_back(42);
    EXPECT_EQ(list.get(), 42);
}

TEST(ListTest, GetOddNumberOfElements) {
    List list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    list.push_back(5);
    EXPECT_EQ(list.get(), 3);
}

TEST(ListTest, GetEvenNumberOfElements) {
    List list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    EXPECT_EQ(list.get(), 2);
}

TEST(ListTest, GetTwoElements) {
    List list;
    list.push_back(10);
    list.push_back(20);
    EXPECT_EQ(list.get(), 10);
}

TEST(ListTest, GetThreeElements) {
    List list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);
    EXPECT_EQ(list.get(), 20);
}

TEST(ListTest, MixedPushFrontBack) {
    List list;
    list.push_front(1);
    list.push_back(2);
    list.push_front(0);
    list.push_back(3);

    EXPECT_FALSE(list.empty());
    EXPECT_EQ(list.get(), 1);
}

TEST(ListTest, SequentialOperations) {
    List list;

    list.push_front(10);
    list.push_back(20);
    list.push_front(5);
    list.push_back(25);

    EXPECT_EQ(list.get(), 10);

    list.pop_front();
    EXPECT_EQ(list.get(), 20);

    list.pop_back();
    EXPECT_EQ(list.get(), 10);

    list.pop_front();
    EXPECT_EQ(list.get(), 20);

    list.pop_back();
    EXPECT_TRUE(list.empty());
}

TEST(ListTest, LargeNumberOfElements) {
    List list;

    for (int i = 0; i < 100; ++i) {
        list.push_back(i);
    }

    EXPECT_EQ(list.get(), 49);
    EXPECT_FALSE(list.empty());

    for (int i = 0; i < 100; ++i) {
        list.pop_front();
    }

    EXPECT_TRUE(list.empty());
}

TEST(ListTest, MixedOperationsStressTest) {
    List list;

    for (int i = 0; i < 50; ++i) {
        if (i % 2 == 0) {
            list.push_front(i);
        } else {
            list.push_back(i);
        }
    }

    EXPECT_FALSE(list.empty());

    for (int i = 0; i < 25; ++i) {
        if (i % 2 == 0) {
            list.pop_front();
        } else {
            list.pop_back();
        }
    }

    EXPECT_FALSE(list.empty());
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}