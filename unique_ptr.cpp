#include <iostream>
#include <memory>

void print(const std::unique_ptr<int>& a, const char* name) {
    std::cout << name << " = " << (a ? *a : 0) << std::endl;
}

int main() {
    std::unique_ptr<int> a;
    print(a, "a");
    a = std::unique_ptr<int>(new int);
    *a = 10;
    print(a, "a");
    std::unique_ptr<int> b(std::move(a));
    print(b, "b");
    print(a, "a");

    return 0;
}
