#include <format>
#include <iostream>
#include <variant>

int main() {
    std::variant<int, float> var1;
    std::variant<int, float> var2;
    var1 = 1;
    var2 = 0.5f;
    std::cout << std::format("var1: int: {}, float: {}", std::holds_alternative<int>(var1) ? std::get<int>(var1) : 0,
                             std::holds_alternative<float>(var1) ? std::get<float>(var1) : 0)
              << std::endl;
    std::cout << std::format("var2: int: {}, float: {}", std::holds_alternative<int>(var2) ? std::get<int>(var2) : 0,
                             std::holds_alternative<float>(var2) ? std::get<float>(var2) : 0)
              << std::endl;
    return 0;
}
