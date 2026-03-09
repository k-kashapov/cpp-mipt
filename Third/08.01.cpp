#include <cstring>
#include <iostream>

class Entity_v1 {
  private:
    int value;

  public:
    Entity_v1(int v) : value(v) {}
    void print() const { std::cout << "Entity_v1 value = " << value << std::endl; }
};

class Entity_v2 {
  public:
    int value;
};

int main() {
    Entity_v1 e1(42);
    e1.print();

    Entity_v2 &e2 = reinterpret_cast<Entity_v2 &>(e1);
    e2.value = 100;
    e1.print();

    int *p = reinterpret_cast<int *>(&e1);
    *p = 200;
    e1.print();

    char *bytes = reinterpret_cast<char *>(&e1);
    int new_value = 300;
    std::memcpy(bytes, &new_value, sizeof(int));
    e1.print();

    return 0;
}
