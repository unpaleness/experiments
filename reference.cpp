#include <iostream>
#include <memory>

class A {
public:
    A(const int newValue) : value(newValue) {}
    int value{0};
};

void changeValue(const int newValue, A& a) {
    a.value = newValue;
}

void userCode(A* const newA) {
    A* const a = newA;
    std::cout << a->value << '\n';
    changeValue(9, *a);
    std::cout << a->value << '\n';
    a = nullptr;
}

int main() {
    auto a1 = std::make_unique<A>(10);
    userCode(a1.get());
    a1 = nullptr;
    return 0;
}
