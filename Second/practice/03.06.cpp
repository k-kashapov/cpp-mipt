#include <cmath>
#include <gtest/gtest.h>
#include <memory>
#include <numbers>
#include <vector>

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

TEST(ShapeTest, TriangleValidConstruction) {
    EXPECT_NO_THROW(Triangle(3, 4, 5));
    EXPECT_NO_THROW(Triangle(5, 12, 13));
    EXPECT_NO_THROW(Triangle(7, 8, 10));
}

TEST(ShapeTest, TriangleInvalidConstruction) {
    EXPECT_THROW(Triangle(0, 4, 5), std::invalid_argument);
    EXPECT_THROW(Triangle(1, 2, 10), std::invalid_argument);
    EXPECT_THROW(Triangle(-1, 2, 3), std::invalid_argument);
}

TEST(ShapeTest, TrianglePerimeter) {
    Triangle t(3, 4, 5);
    EXPECT_DOUBLE_EQ(t.perimeter(), 12.0);

    Triangle t2(5, 12, 13);
    EXPECT_DOUBLE_EQ(t2.perimeter(), 30.0);
}

TEST(ShapeTest, TriangleArea) {
    Triangle t(3, 4, 5);
    EXPECT_DOUBLE_EQ(t.area(), 6.0);

    Triangle t2(6, 8, 10);
    EXPECT_DOUBLE_EQ(t2.area(), 24.0);
}

TEST(ShapeTest, SquareValidConstruction) {
    EXPECT_NO_THROW(Square(1));
    EXPECT_NO_THROW(Square(2.5));
    EXPECT_NO_THROW(Square(100));
}

TEST(ShapeTest, SquareInvalidConstruction) {
    EXPECT_THROW(Square(0), std::invalid_argument);
    EXPECT_THROW(Square(-5), std::invalid_argument);
}

TEST(ShapeTest, SquarePerimeter) {
    Square s(5);
    EXPECT_DOUBLE_EQ(s.perimeter(), 20.0);

    Square s2(2.5);
    EXPECT_DOUBLE_EQ(s2.perimeter(), 10.0);
}

TEST(ShapeTest, SquareArea) {
    Square s(4);
    EXPECT_DOUBLE_EQ(s.area(), 16.0);

    Square s2(3.5);
    EXPECT_DOUBLE_EQ(s2.area(), 12.25);
}

TEST(ShapeTest, CircleValidConstruction) {
    EXPECT_NO_THROW(Circle(1));
    EXPECT_NO_THROW(Circle(2.5));
    EXPECT_NO_THROW(Circle(100));
}

TEST(ShapeTest, CircleInvalidConstruction) {
    EXPECT_THROW(Circle(0), std::invalid_argument);
    EXPECT_THROW(Circle(-2), std::invalid_argument);
}

TEST(ShapeTest, CirclePerimeter) {
    Circle c(1);
    EXPECT_NEAR(c.perimeter(), 2 * std::numbers::pi, 1e-10);

    Circle c2(2.5);
    EXPECT_NEAR(c2.perimeter(), 5 * std::numbers::pi, 1e-10);
}

TEST(ShapeTest, CircleArea) {
    Circle c(2);
    EXPECT_NEAR(c.area(), 4 * std::numbers::pi, 1e-10);

    Circle c2(3);
    EXPECT_NEAR(c2.area(), 9 * std::numbers::pi, 1e-10);
}

TEST(ShapePolymorphismTest, VectorOfShapes) {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Triangle>(3, 4, 5));
    shapes.push_back(std::make_unique<Square>(5));
    shapes.push_back(std::make_unique<Circle>(1));

    EXPECT_EQ(shapes.size(), 3);

    double total_area = 0;
    for (const auto &shape : shapes) {
        total_area += shape->area();
    }

    EXPECT_NEAR(total_area, 6.0 + 25.0 + std::numbers::pi, 1e-10);
}

TEST(ShapePolymorphismTest, PerimeterCalculationThroughBase) {
    std::unique_ptr<Shape> triangle = std::make_unique<Triangle>(3, 4, 5);
    std::unique_ptr<Shape> square = std::make_unique<Square>(4);
    std::unique_ptr<Shape> circle = std::make_unique<Circle>(2);

    EXPECT_DOUBLE_EQ(triangle->perimeter(), 12.0);
    EXPECT_DOUBLE_EQ(square->perimeter(), 16.0);
    EXPECT_NEAR(circle->perimeter(), 4 * std::numbers::pi, 1e-10);
}

TEST(ShapePolymorphismTest, AreaCalculationThroughBase) {
    std::unique_ptr<Shape> triangle = std::make_unique<Triangle>(3, 4, 5);
    std::unique_ptr<Shape> square = std::make_unique<Square>(4);
    std::unique_ptr<Shape> circle = std::make_unique<Circle>(2);

    EXPECT_DOUBLE_EQ(triangle->area(), 6.0);
    EXPECT_DOUBLE_EQ(square->area(), 16.0);
    EXPECT_NEAR(circle->area(), 4 * std::numbers::pi, 1e-10);
}

TEST(ShapePolymorphismTest, DynamicCastVerification) {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Triangle>(3, 4, 5));
    shapes.push_back(std::make_unique<Square>(5));
    shapes.push_back(std::make_unique<Circle>(1));

    Triangle *triangle = dynamic_cast<Triangle *>(shapes[0].get());
    Square *square = dynamic_cast<Square *>(shapes[1].get());
    Circle *circle = dynamic_cast<Circle *>(shapes[2].get());

    EXPECT_NE(triangle, nullptr);
    EXPECT_NE(square, nullptr);
    EXPECT_NE(circle, nullptr);

    EXPECT_DOUBLE_EQ(triangle->area(), 6.0);
    EXPECT_DOUBLE_EQ(square->area(), 25.0);
    EXPECT_NEAR(circle->area(), std::numbers::pi, 1e-10);
}

TEST(ShapePolymorphismTest, MixedShapesCollection) {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Triangle>(3, 4, 5));
    shapes.push_back(std::make_unique<Square>(2.5));
    shapes.push_back(std::make_unique<Circle>(3));
    shapes.push_back(std::make_unique<Triangle>(5, 12, 13));
    shapes.push_back(std::make_unique<Square>(10));

    double total_perimeter = 0;
    double total_area = 0;

    for (const auto &shape : shapes) {
        total_perimeter += shape->perimeter();
        total_area += shape->area();
    }

    EXPECT_GT(total_perimeter, 0);
    EXPECT_GT(total_area, 0);

    EXPECT_NEAR(total_area, 6.0 + 30.0 + 6.25 + 9 * std::numbers::pi + 100, 1e-10);
}

TEST(ShapePolymorphismTest, EmptyVector) {
    std::vector<std::unique_ptr<Shape>> shapes;
    EXPECT_TRUE(shapes.empty());
}

TEST(ShapePolymorphismTest, MoveSemantics) {
    std::vector<std::unique_ptr<Shape>> shapes;
    auto triangle = std::make_unique<Triangle>(3, 4, 5);
    auto square = std::make_unique<Square>(4);

    shapes.push_back(std::move(triangle));
    shapes.push_back(std::move(square));

    EXPECT_EQ(shapes.size(), 2);
    EXPECT_DOUBLE_EQ(shapes[0]->area(), 6.0);
    EXPECT_DOUBLE_EQ(shapes[1]->area(), 16.0);
}

TEST(ShapeEdgeCases, VerySmallShapes) {
    Triangle small_triangle(1e-10, 1e-10, 1e-10);
    Square small_square(1e-10);
    Circle small_circle(1e-10);

    EXPECT_GT(small_triangle.area(), 0);
    EXPECT_GT(small_square.area(), 0);
    EXPECT_GT(small_circle.area(), 0);
}

TEST(ShapeEdgeCases, LargeShapes) {
    Triangle large_triangle(1000, 1000, 1000);
    Square large_square(1000);
    Circle large_circle(1000);

    EXPECT_GT(large_triangle.area(), 0);
    EXPECT_DOUBLE_EQ(large_square.area(), 1000000.0);
    EXPECT_NEAR(large_circle.area(), 1000000 * std::numbers::pi, 1e-10);
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
