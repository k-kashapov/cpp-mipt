#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

class Entity {
  private:
    int internal_data = 42;
    std::string secret_message = "classified";

    std::string test_v1() const { return "test_v1_result: " + std::to_string(internal_data); }

    int test_v2(int multiplier) const { return internal_data * multiplier; }

    void test_v3(const std::string &input) { secret_message = input; }

    std::string test_v4() const { return secret_message; }

  public:
    friend class Tester_v1;
    friend class Tester_v2;

    Entity() = default;
};

class Tester_v1 {
  private:
    static std::string call_test_v1(const Entity &entity) { return entity.test_v1(); }

  public:
    static std::string test(const Entity &entity) { return call_test_v1(entity); }
};

class Tester_v2 {
  private:
    static int call_test_v2(const Entity &entity, int multiplier) {
        return entity.test_v2(multiplier);
    }

  public:
    static int test(const Entity &entity, int multiplier) {
        return call_test_v2(entity, multiplier);
    }
};

TEST(Test04, TesterV1Access) {
    Entity entity;
    std::string result = Tester_v1::test(entity);
    EXPECT_EQ(result, "test_v1_result: 42");
}

TEST(Test04, TesterV2Access) {
    Entity entity;
    int result1 = Tester_v2::test(entity, 2);
    int result2 = Tester_v2::test(entity, 3);
    int result3 = Tester_v2::test(entity, 0);
    int result4 = Tester_v2::test(entity, -1);

    EXPECT_EQ(result1, 84);
    EXPECT_EQ(result2, 126);
    EXPECT_EQ(result3, 0);
    EXPECT_EQ(result4, -42);
}

TEST(Test04, MultipleTesterInstances) {
    Entity entity1;
    Entity entity2;

    std::string result1 = Tester_v1::test(entity1);
    std::string result2 = Tester_v1::test(entity2);

    EXPECT_EQ(result1, result2);
    EXPECT_EQ(result1, "test_v1_result: 42");
}

TEST(Test04, TesterIsolation) {
    Entity entity;

    std::string v1_result = Tester_v1::test(entity);
    int v2_result = Tester_v2::test(entity, 1);

    EXPECT_EQ(v1_result, "test_v1_result: 42");
    EXPECT_EQ(v2_result, 42);
}

TEST(Test04, TesterV2EdgeCases) {
    Entity entity;

    EXPECT_EQ(Tester_v2::test(entity, 1), 42);
    EXPECT_EQ(Tester_v2::test(entity, 100), 4200);
    EXPECT_EQ(Tester_v2::test(entity, -2), -84);
}

TEST(Test04, TesterV1Consistency) {
    Entity entity;

    for (int i = 0; i < 10; ++i) {
        std::string result = Tester_v1::test(entity);
        EXPECT_EQ(result, "test_v1_result: 42");
    }
}

TEST(Test04, TesterV2Consistency) {
    Entity entity;

    for (int i = 1; i <= 5; ++i) {
        int result = Tester_v2::test(entity, i);
        EXPECT_EQ(result, 42 * i);
    }
}

TEST(Test04, AttorneyClientPatternEnforcement) {
    Entity entity;

    std::string v1_result = Tester_v1::test(entity);
    int v2_result = Tester_v2::test(entity, 3);

    EXPECT_EQ(v1_result, "test_v1_result: 42");
    EXPECT_EQ(v2_result, 126);
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
