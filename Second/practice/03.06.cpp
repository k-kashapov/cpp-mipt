#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <numbers>
#include <vector>

// Abstract base class
class Shape {
  public:
    virtual ~Shape() = default;
    virtual double perimeter() const = 0;
    virtual double area() const = 0;
};

class Triangle final : public Shape {
  private:
    double side_a;
    double side_b;
    double side_c;

  public:
    Triangle(double a, double b, double c) : side_a(a), side_b(b), side_c(c) {
        if (a <= 0 || b <= 0 || c <= 0) {
            throw std::invalid_argument("All sides must be positive");
        }
        if (a + b <= c || a + c <= b || b + c <= a) {
            throw std::invalid_argument("Triangle inequality violated");
        }
    }

    double perimeter() const override { return side_a + side_b + side_c; }

    double area() const override {
        double s = perimeter() / 2.0;
        return std::sqrt(s * (s - side_a) * (s - side_b) * (s - side_c));
    }
};

class Square final : public Shape {
  private:
    double side;

  public:
    Square(double s) : side(s) {
        if (s <= 0) {
            throw std::invalid_argument("Side must be positive");
        }
    }

    double perimeter() const override { return 4 * side; }

    double area() const override { return side * side; }
};

class Circle final : public Shape {
  private:
    double radius;

  public:
    Circle(double r) : radius(r) {
        if (r <= 0) {
            throw std::invalid_argument("Radius must be positive");
        }
    }

    double perimeter() const override { return 2 * std::numbers::pi * radius; }

    double area() const override { return std::numbers::pi * radius * radius; }
};

int main() {
    // Create shapes through base class pointers
    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.push_back(std::make_unique<Triangle>(3, 4, 5));
    shapes.push_back(std::make_unique<Square>(5));
    shapes.push_back(std::make_unique<Circle>(1));
    shapes.push_back(std::make_unique<Triangle>(5, 12, 13));
    shapes.push_back(std::make_unique<Square>(2.5));
    shapes.push_back(std::make_unique<Circle>(2));

    // Test polymorphism
    double total_perimeter = 0;
    double total_area = 0;

    std::cout << "Shapes collection:\n";
    for (size_t i = 0; i < shapes.size(); ++i) {
        double p = shapes[i]->perimeter();
        double a = shapes[i]->area();

        std::cout << "Shape " << i + 1 << ": ";
        std::cout << "Perimeter = " << p << ", Area = " << a << "\n";

        total_perimeter += p;
        total_area += a;

        // Verify that we're calling the correct overridden methods
        if (dynamic_cast<Triangle *>(shapes[i].get())) {
            assert(p > 0 && a > 0);
        } else if (dynamic_cast<Square *>(shapes[i].get())) {
            assert(p > 0 && a > 0);
        } else if (dynamic_cast<Circle *>(shapes[i].get())) {
            assert(p > 0 && a > 0);
        }
    }

    std::cout << "\nTotal perimeter: " << total_perimeter << "\n";
    std::cout << "Total area: " << total_area << "\n";

    // Test specific shape properties
    Triangle triangle(3, 4, 5);
    assert(std::abs(triangle.area() - 6.0) < 1e-10);

    Square square(4);
    assert(square.area() == 16.0);

    Circle circle(2);
    assert(std::abs(circle.area() - 4 * std::numbers::pi) < 1e-10);

    std::cout << "\nAll tests passed!\n";
    std::cout << "Polymorphism working correctly with final and override specifiers.\n";

    return 0;
}
