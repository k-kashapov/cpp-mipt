#include <iostream>
#include <string>

struct Person {
    std::string name() const { return ""; };
    int grade() const { return 0; };
    int salary() const { return 0; };
    int id() const { return 0; };
};

enum class Status : bool { success = true, failure = false };

void save(Status, int);

void test(Person const &person) {
    // Any member of Person may throw exception
    // std::cout will throw if nullptr is returned as name
    std ::cout << " test : " << person.name() << '\n';
    if (person.grade() == 10 || person.salary() > 1'000'000) {
        // save function may throw an exception inside it
        save(Status::success, person.id());
    } else {
        save(Status::failure, person.id());
    }
}
