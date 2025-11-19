#include <cmath>
#include <iostream>

static inline bool isZero(double x) {
    const double epsilon = 1e-10;
    return std::abs(x) < epsilon;
}

int main() {
    double a = NAN, b = NAN, c = NAN;
    std::cin >> a >> b >> c;

    if (isZero(a)) {
        if (isZero(b)) {
            if (isZero(c))
                std::cout << "Solution: R\n";
            else
                std::cout << "No solution\n";
        } else {
            double root = -c / b;
            std::cout << root;
        }
    } else {
        double discriminant = b * b - 4 * a * c;
        if (isZero(discriminant)) {
            double root = -b / (2 * a);
            std::cout << root;
        } else if (discriminant < 0) {
            std::cout << "No real roots\n";
        } else {
            double root1 = (-b - std::sqrt(discriminant)) / (2 * a);
            double root2 = (-b + std::sqrt(discriminant)) / (2 * a);
            std::cout << root1 << " " << root2 << "\n";
        }
    }

    return 0;
}
