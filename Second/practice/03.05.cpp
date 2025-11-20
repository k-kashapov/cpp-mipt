#include <gtest/gtest.h>
#include <string>

class Entity_v1 {
  public:
    virtual ~Entity_v1() = default;
    virtual std::string test() const { return "Entity_v1::test"; }
};

class Entity_v2 {
  public:
    virtual ~Entity_v2() = default;
    virtual std::string test() const { return "Entity_v2::test"; }
};

class Adapter_v1 : public Entity_v1 {
  public:
    virtual std::string test_v1() const { return Entity_v1::test(); }
    std::string test() const override { return test_v1(); }
};

class Adapter_v2 : public Entity_v2 {
  public:
    virtual std::string test_v2() const { return Entity_v2::test(); }
    std::string test() const override { return test_v2(); }
};

class Client : public Adapter_v1, public Adapter_v2 {
  public:
    std::string test_v1() const override { return "Client::test_v1_override"; }
    std::string test_v2() const override { return "Client::test_v2_override"; }
    std::string test() const override { return "Client::test_override"; }
};

TEST(MultipleInheritanceAdapterTest, ClientDirectAccess) {
    Client client;
    EXPECT_EQ(client.test_v1(), "Client::test_v1_override");
    EXPECT_EQ(client.test_v2(), "Client::test_v2_override");
    EXPECT_EQ(client.test(), "Client::test_override");
}

TEST(MultipleInheritanceAdapterTest, AdapterV1Pointer) {
    Client client;
    Adapter_v1 *adapter1 = &client;
    EXPECT_EQ(adapter1->test_v1(), "Client::test_v1_override");
    EXPECT_EQ(adapter1->test(), "Client::test_override");
}

TEST(MultipleInheritanceAdapterTest, AdapterV2Pointer) {
    Client client;
    Adapter_v2 *adapter2 = &client;
    EXPECT_EQ(adapter2->test_v2(), "Client::test_v2_override");
    EXPECT_EQ(adapter2->test(), "Client::test_override");
}

TEST(MultipleInheritanceAdapterTest, EntityV1Pointer) {
    Client client;
    Entity_v1 *base1 = &client;
    EXPECT_EQ(base1->test(), "Client::test_override");
}

TEST(MultipleInheritanceAdapterTest, EntityV2Pointer) {
    Client client;
    Entity_v2 *base2 = &client;
    EXPECT_EQ(base2->test(), "Client::test_override");
}

TEST(MultipleInheritanceAdapterTest, SeparateMethodOverrides) {
    Client client;

    std::string v1_result = client.test_v1();
    std::string v2_result = client.test_v2();

    EXPECT_NE(v1_result, v2_result);
    EXPECT_EQ(v1_result, "Client::test_v1_override");
    EXPECT_EQ(v2_result, "Client::test_v2_override");
}

TEST(MultipleInheritanceAdapterTest, OriginalEntityBehavior) {
    Entity_v1 entity1;
    Entity_v2 entity2;

    EXPECT_EQ(entity1.test(), "Entity_v1::test");
    EXPECT_EQ(entity2.test(), "Entity_v2::test");
}

TEST(MultipleInheritanceAdapterTest, AdapterDefaultBehavior) {
    Adapter_v1 adapter1;
    Adapter_v2 adapter2;

    EXPECT_EQ(adapter1.test_v1(), "Entity_v1::test");
    EXPECT_EQ(adapter2.test_v2(), "Entity_v2::test");
    EXPECT_EQ(adapter1.test(), "Entity_v1::test");
    EXPECT_EQ(adapter2.test(), "Entity_v2::test");
}

TEST(MultipleInheritanceAdapterTest, MultipleClientInstances) {
    Client client1;
    Client client2;

    EXPECT_EQ(client1.test_v1(), client2.test_v1());
    EXPECT_EQ(client1.test_v2(), client2.test_v2());
    EXPECT_EQ(client1.test(), client2.test());
}

TEST(MultipleInheritanceAdapterTest, PolymorphicBehavior) {
    Client client;

    Adapter_v1 *a1 = &client;
    Adapter_v2 *a2 = &client;
    Entity_v1 *e1 = &client;
    Entity_v2 *e2 = &client;

    EXPECT_EQ(a1->test_v1(), "Client::test_v1_override");
    EXPECT_EQ(a2->test_v2(), "Client::test_v2_override");
    EXPECT_EQ(e1->test(), "Client::test_override");
    EXPECT_EQ(e2->test(), "Client::test_override");
}

TEST(MultipleInheritanceAdapterTest, MethodSignatureSeparation) {
    Client client;

    EXPECT_EQ(client.Adapter_v1::test(), "Client::test_override");
    EXPECT_EQ(client.Adapter_v2::test(), "Client::test_override");
}

TEST(MultipleInheritanceAdapterTest, AdapterChain) {
    class ExtendedClient : public Client {
      public:
        std::string test_v1() const override { return "ExtendedClient::test_v1"; }
        std::string test_v2() const override { return "ExtendedClient::test_v2"; }
    };

    ExtendedClient ext_client;

    EXPECT_EQ(ext_client.test_v1(), "ExtendedClient::test_v1");
    EXPECT_EQ(ext_client.test_v2(), "ExtendedClient::test_v2");

    Adapter_v1 *a1 = &ext_client;
    Adapter_v2 *a2 = &ext_client;

    EXPECT_EQ(a1->test_v1(), "ExtendedClient::test_v1");
    EXPECT_EQ(a2->test_v2(), "ExtendedClient::test_v2");
}

TEST(MultipleInheritanceAdapterTest, ConstCorrectness) {
    const Client client;

    EXPECT_EQ(client.test_v1(), "Client::test_v1_override");
    EXPECT_EQ(client.test_v2(), "Client::test_v2_override");
    EXPECT_EQ(client.test(), "Client::test_override");
}

TEST(MultipleInheritanceAdapterTest, ReferenceSemantics) {
    Client client;

    Adapter_v1 &ref1 = client;
    Adapter_v2 &ref2 = client;
    Entity_v1 &eref1 = client;
    Entity_v2 &eref2 = client;

    EXPECT_EQ(ref1.test_v1(), "Client::test_v1_override");
    EXPECT_EQ(ref2.test_v2(), "Client::test_v2_override");
    EXPECT_EQ(eref1.test(), "Client::test_override");
    EXPECT_EQ(eref2.test(), "Client::test_override");
}

TEST(MultipleInheritanceAdapterTest, NoAmbiguityInAdaptedMethods) {
    Client client;

    std::string v1_result = client.test_v1();
    std::string v2_result = client.test_v2();

    EXPECT_NE(v1_result, v2_result);
}

TEST(MultipleInheritanceAdapterTest, OriginalTestMethodOverride) {
    Client client;

    Entity_v1 *e1 = &client;
    Entity_v2 *e2 = &client;

    EXPECT_EQ(e1->test(), "Client::test_override");
    EXPECT_EQ(e2->test(), "Client::test_override");
}

TEST(MultipleInheritanceAdapterTest, DynamicCastSupport) {
    Client client;

    Entity_v1 *e1 = &client;
    Entity_v2 *e2 = &client;

    Client *cast1 = dynamic_cast<Client *>(e1);
    Client *cast2 = dynamic_cast<Client *>(e2);

    EXPECT_NE(cast1, nullptr);
    EXPECT_NE(cast2, nullptr);
    EXPECT_EQ(cast1, cast2);
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
