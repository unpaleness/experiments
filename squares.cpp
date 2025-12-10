#include <exception>
#include <iomanip>
#include <iostream>
#include <string>

std::string Usage() {
    return "Usage: <executable> tens figures\n";
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << Usage() << std::endl;
        return 1;
    }
    int tens;
    size_t figures;
    try {
        tens = std::stoi(argv[1]);
        figures = std::stoi(argv[2]);
    } catch (std::exception& e) {
        std::cerr << "Error in " << e.what() << std::endl;
        return 1;
    }
    std::cout << std::setw(figures + 1) << '|';
    for (int i = 0; i < 10; ++i) {
        std::cout << std::setw(figures + 1) << i;
    }
    std::cout << std::endl;
    std::cout << std::setfill('-');
    std::cout << std::setw(figures + 1) << '+';
    for (int i = 0; i < 10; ++i) {
        std::cout << std::setw(figures + 1) << '-';
    }
    std::cout << std::endl;
    std::cout << std::setfill(' ');
    for (int j = 0; j < tens; ++j) {
        std::cout << std::left << std::setw(figures) << j << '|';
        for (int i = 0; i < 10; ++i) {
            std::string square(std::to_string((j * 10 + i) * (j * 10 + i)));
            std::cout << std::right << std::setw(figures + 1)
                      << square.substr((square.size() > figures ? square.size() - figures : 0));
        }
        std::cout << std::endl;
    }
    return 0;
}