#include <cassert>
#include <iostream>
#include <string>

class Entity {
  private:
    int internal_data = 42;

    // Private methods to test
    std::string test_v1() const { return "test_v1_result: " + std::to_string(internal_data); }

    int test_v2(int multiplier) const { return internal_data * multiplier; }

  public:
    // Grant limited access to specific testers using Attorney-Client pattern
    friend class Tester_v1;
    friend class Tester_v2;

    Entity() = default;
};

// Attorney class for test_v1
class Tester_v1 {
  private:
    static std::string call_test_v1(const Entity &entity) { return entity.test_v1(); }

  public:
    static void test(const Entity &entity) {
        std::string result = call_test_v1(entity);
        std::cout << "Tester_v1: " << result << "\n";
        assert(result == "test_v1_result: 42");
    }
};

// Attorney class for test_v2
class Tester_v2 {
  private:
    static int call_test_v2(const Entity &entity, int multiplier) {
        return entity.test_v2(multiplier);
    }

  public:
    static void test(const Entity &entity) {
        int result = call_test_v2(entity, 2);
        std::cout << "Tester_v2: " << result << "\n";
        assert(result == 84);

        result = call_test_v2(entity, 3);
        std::cout << "Tester_v2: " << result << "\n";
        assert(result == 126);
    }
};

int main() {
    Entity entity;

    // Each tester can only access their specific method
    Tester_v1::test(entity);
    Tester_v2::test(entity);

    // Regular code cannot access private methods
    // entity.test_v1();  // This would not compile
    // entity.test_v2(2); // This would not compile

    std::cout << "Attorney-Client pattern test passed!\n";
    std::cout << "Each tester has access only to their specific private method.\n";

    return 0;
}
