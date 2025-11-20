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
    friend class Tester_v3;
    friend class Tester_v4;

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

class Tester_v3 {
  private:
    static void call_test_v3(Entity &entity, const std::string &input) { entity.test_v3(input); }

  public:
    static void test(Entity &entity, const std::string &input) { call_test_v3(entity, input); }
};

class Tester_v4 {
  private:
    static std::string call_test_v4(const Entity &entity) { return entity.test_v4(); }

  public:
    static std::string test(const Entity &entity) { return call_test_v4(entity); }
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

TEST(Test04, TesterV3V4Combined) {
    Entity entity;

    std::string initial = Tester_v4::test(entity);
    EXPECT_EQ(initial, "classified");

    Tester_v3::test(entity, "new_secret");

    std::string updated = Tester_v4::test(entity);
    EXPECT_EQ(updated, "new_secret");
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

TEST(Test04, TesterV3Modification) {
    Entity entity;

    Tester_v3::test(entity, "modified_by_tester_v3");
    std::string result = Tester_v4::test(entity);

    EXPECT_EQ(result, "modified_by_tester_v3");
}

TEST(Test04, TesterV4InitialState) {
    Entity entity;
    std::string result = Tester_v4::test(entity);
    EXPECT_EQ(result, "classified");
}

TEST(Test04, MultipleModifications) {
    Entity entity;

    Tester_v3::test(entity, "first");
    EXPECT_EQ(Tester_v4::test(entity), "first");

    Tester_v3::test(entity, "second");
    EXPECT_EQ(Tester_v4::test(entity), "second");

    Tester_v3::test(entity, "third");
    EXPECT_EQ(Tester_v4::test(entity), "third");
}

TEST(Test04, EmptyStringModification) {
    Entity entity;

    Tester_v3::test(entity, "");
    std::string result = Tester_v4::test(entity);

    EXPECT_EQ(result, "");
}

TEST(Test04, LongStringModification) {
    Entity entity;
    std::string long_string(100, 'x');

    Tester_v3::test(entity, long_string);
    std::string result = Tester_v4::test(entity);

    EXPECT_EQ(result, long_string);
}

TEST(Test04, TesterV2EdgeCases) {
    Entity entity;

    EXPECT_EQ(Tester_v2::test(entity, 1), 42);
    EXPECT_EQ(Tester_v2::test(entity, 100), 4200);
    EXPECT_EQ(Tester_v2::test(entity, -2), -84);
}

TEST(Test04, SeparateEntityInstances) {
    Entity entity1;
    Entity entity2;

    Tester_v3::test(entity1, "entity1_secret");
    Tester_v3::test(entity2, "entity2_secret");

    EXPECT_EQ(Tester_v4::test(entity1), "entity1_secret");
    EXPECT_EQ(Tester_v4::test(entity2), "entity2_secret");
}

TEST(Test04, AllTestersOnSameEntity) {
    Entity entity;

    std::string v1_result = Tester_v1::test(entity);
    int v2_result = Tester_v2::test(entity, 2);
    Tester_v3::test(entity, "combined_test");
    std::string v4_result = Tester_v4::test(entity);

    EXPECT_EQ(v1_result, "test_v1_result: 42");
    EXPECT_EQ(v2_result, 84);
    EXPECT_EQ(v4_result, "combined_test");
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

TEST(Test04, TesterSpecificAccess) {
    Entity entity;

    // Tester_v1 can only access test_v1
    std::string v1_result = Tester_v1::test(entity);
    EXPECT_EQ(v1_result, "test_v1_result: 42");

    // Tester_v2 can only access test_v2
    int v2_result = Tester_v2::test(entity, 2);
    EXPECT_EQ(v2_result, 84);

    // Tester_v3 can only access test_v3
    Tester_v3::test(entity, "v3_access");

    // Tester_v4 can only access test_v4
    std::string v4_result = Tester_v4::test(entity);
    EXPECT_EQ(v4_result, "v3_access");
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
