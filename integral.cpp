#include <iomanip>
#include <iostream>

double fun(double x) {
    return sqrt(4.f - x * x) * (x * x * x * cos(x / 2.f) + 0.5f);
}

int main() {
    std::cout << std::fixed << std::setprecision(30);
    constexpr double x1 = -2.f;
    constexpr double x2 = 2.f;
    constexpr double step = 0.00000001f;
    double result = 0.f;
    double x = x1 + 0.5f * step;
    while (x < x2) {
        const double tmp = fun(x);
        // std::cout << x << ' ' << tmp << "\n";
        result += tmp * step;
        x += step;
    }
    std::cout << result << "\n";
    std::cout << M_PI << "\n";
    return 0;
}
