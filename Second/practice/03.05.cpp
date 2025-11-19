#include <cassert>
#include <iostream>
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

// Adapter for Entity_v1 to change method signature
class Adapter_v1 : public Entity_v1 {
  public:
    virtual std::string test_v1() const { return Entity_v1::test(); }

    // Override the original test method if needed
    std::string test() const override { return test_v1(); }
};

// Adapter for Entity_v2 to change method signature
class Adapter_v2 : public Entity_v2 {
  public:
    virtual std::string test_v2() const { return Entity_v2::test(); }

    // Override the original test method if needed
    std::string test() const override { return test_v2(); }
};

// Client class that can separately override both methods
class Client : public Adapter_v1, public Adapter_v2 {
  public:
    // Separate overrides for each adapted method
    std::string test_v1() const override { return "Client::test_v1_override"; }

    std::string test_v2() const override { return "Client::test_v2_override"; }

    // We can still override the original methods if needed
    std::string test() const override { return "Client::test_override"; }
};

int main() {
    Client client;

    // Test the separately overridden methods
    std::cout << "Client test_v1: " << client.test_v1() << "\n";
    std::cout << "Client test_v2: " << client.test_v2() << "\n";

    assert(client.test_v1() == "Client::test_v1_override");
    assert(client.test_v2() == "Client::test_v2_override");

    // Test through base class pointers
    Adapter_v1 *adapter1 = &client;
    Adapter_v2 *adapter2 = &client;

    std::cout << "Through Adapter_v1: " << adapter1->test_v1() << "\n";
    std::cout << "Through Adapter_v2: " << adapter2->test_v2() << "\n";

    assert(adapter1->test_v1() == "Client::test_v1_override");
    assert(adapter2->test_v2() == "Client::test_v2_override");

    // The original test methods (with same signature) can still be called
    Entity_v1 *base1 = &client;
    Entity_v2 *base2 = &client;

    std::cout << "Through Entity_v1: " << base1->test() << "\n";
    std::cout << "Through Entity_v2: " << base2->test() << "\n";

    std::cout << "All tests passed!\n";
    std::cout << "Successfully separated overrides for methods with same signature.\n";

    return 0;
}
