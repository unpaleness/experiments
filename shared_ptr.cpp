#include <iostream>
#include <memory>

class A {
public:
    A(const int newValue) : value(newValue) {}
    int value{0};
};

int main() {
    auto a1 = std::make_shared<A>(10);
    std::weak_ptr<A> a2 = a1;
    std::cout << "auto a1 = std::make_shared<A>(10);\nstd::weak_ptr<A> a2 = a1;\n";
    std::cout << "a1->value = " << a1->value << '\n';
    std::cout << "a2.lock()->value = " << a2.lock()->value << '\n';
    a1.reset();
    std::cout << "a1.reset();\n";
    std::cout << "a1.get() " << (a1 ? "!=" : "==") << " nullptr\n";
    if (a1) {
        std::cout << "a1->value = " << a1->value << '\n';
    }
    std::cout << "a2.expired() == " << (a2.expired() ? "true" : "false") << '\n';
    if (!a2.expired()) {
        std::cout << "a2.lock()->value = " << a2.lock()->value << '\n';
    }
    auto a3 = a2.lock();
    std::cout << "auto a3 = a2.lock();\n";
    std::cout << "a2.expired() == " << (a2.expired() ? "true" : "false") << '\n';
    std::cout << "a3.get() " << (a3 ? "!=" : "==") << " nullptr\n";
    return 0;
}