#include <cmath>
#include <gtest/gtest.h>
#include <numbers>

class Triangle {
  private:
    double side_a;
    double side_b;
    double side_c;

  public:
    Triangle(double a, double b, double c) : side_a(a), side_b(b), side_c(c) {
        if (a <= 0 || b <= 0 || c <= 0) {
            throw std::invalid_argument("All sides must be positive");
        }
        if (a + b < c || a + c < b || b + c < a) {
            throw std::invalid_argument("Triangle inequality violated");
        }
    }

    double perimeter() const { return side_a + side_b + side_c; }

    double area() const {
        double s = perimeter() / 2.0;
        return std::sqrt(s * (s - side_a) * (s - side_b) * (s - side_c));
    }
};

class Square {
  private:
    double side;

  public:
    Square(double s) : side(s) {
        if (s < 0) {
            throw std::invalid_argument("Side must be positive");
        }
    }

    double perimeter() const { return 4 * side; }

    double area() const { return side * side; }
};

class Circle {
  private:
    double radius;

  public:
    Circle(double r) : radius(r) {
        if (r < 0) {
            throw std::invalid_argument("Radius must be positive");
        }
    }

    double perimeter() const { return 2 * std::numbers::pi * radius; }

    double area() const { return std::numbers::pi * radius * radius; }
};

TEST(TriangleTest, ConstructorValidTriangle) {
    EXPECT_NO_THROW(Triangle(3, 4, 5));
    EXPECT_NO_THROW(Triangle(5, 12, 13));
    EXPECT_NO_THROW(Triangle(7, 8, 10));
}

TEST(TriangleTest, ConstructorInvalidTriangle) {
    EXPECT_THROW(Triangle(0, 4, 5), std::invalid_argument);
    EXPECT_THROW(Triangle(3, -4, 5), std::invalid_argument);
    EXPECT_THROW(Triangle(1, 2, 10), std::invalid_argument);
    EXPECT_THROW(Triangle(1, 10, 2), std::invalid_argument);
    EXPECT_THROW(Triangle(10, 1, 2), std::invalid_argument);
}

TEST(TriangleTest, PerimeterCalculation) {
    Triangle t1(3, 4, 5);
    Triangle t2(5, 12, 13);
    Triangle t3(7, 8, 10);

    EXPECT_DOUBLE_EQ(t1.perimeter(), 12.0);
    EXPECT_DOUBLE_EQ(t2.perimeter(), 30.0);
    EXPECT_DOUBLE_EQ(t3.perimeter(), 25.0);
}

TEST(TriangleTest, AreaCalculation) {
    Triangle t1(3, 4, 5);
    Triangle t2(5, 12, 13);
    Triangle t3(6, 8, 10);

    EXPECT_DOUBLE_EQ(t1.area(), 6.0);
    EXPECT_DOUBLE_EQ(t2.area(), 30.0);
    EXPECT_DOUBLE_EQ(t3.area(), 24.0);
}

TEST(TriangleTest, EquilateralTriangle) {
    Triangle t(5, 5, 5);
    EXPECT_DOUBLE_EQ(t.perimeter(), 15.0);
    EXPECT_NEAR(t.area(), 10.825317547305483, 1e-10);
}

TEST(TriangleTest, IsoscelesTriangle) {
    Triangle t(5, 5, 6);
    EXPECT_DOUBLE_EQ(t.perimeter(), 16.0);
    EXPECT_NEAR(t.area(), 12.0, 1e-10);
}

TEST(SquareTest, ConstructorValidSquare) {
    EXPECT_NO_THROW(Square(1));
    EXPECT_NO_THROW(Square(2.5));
    EXPECT_NO_THROW(Square(100));
}

TEST(SquareTest, ConstructorInvalidSquare) {
    EXPECT_THROW(Square(-1), std::invalid_argument);
    EXPECT_THROW(Square(-5.5), std::invalid_argument);
}

TEST(SquareTest, PerimeterCalculation) {
    Square s1(1);
    Square s2(2.5);
    Square s3(10);

    EXPECT_DOUBLE_EQ(s1.perimeter(), 4.0);
    EXPECT_DOUBLE_EQ(s2.perimeter(), 10.0);
    EXPECT_DOUBLE_EQ(s3.perimeter(), 40.0);
}

TEST(SquareTest, AreaCalculation) {
    Square s1(1);
    Square s2(2.5);
    Square s3(10);

    EXPECT_DOUBLE_EQ(s1.area(), 1.0);
    EXPECT_DOUBLE_EQ(s2.area(), 6.25);
    EXPECT_DOUBLE_EQ(s3.area(), 100.0);
}

TEST(SquareTest, LargeValues) {
    Square s(1000);
    EXPECT_DOUBLE_EQ(s.perimeter(), 4000.0);
    EXPECT_DOUBLE_EQ(s.area(), 1000000.0);
}

TEST(CircleTest, ConstructorValidCircle) {
    EXPECT_NO_THROW(Circle(1));
    EXPECT_NO_THROW(Circle(2.5));
    EXPECT_NO_THROW(Circle(100));
}

TEST(CircleTest, ConstructorInvalidCircle) {
    EXPECT_THROW(Circle(-1), std::invalid_argument);
    EXPECT_THROW(Circle(-5.5), std::invalid_argument);
}

TEST(CircleTest, PerimeterCalculation) {
    Circle c1(1);
    Circle c2(2.5);
    Circle c3(10);

    EXPECT_NEAR(c1.perimeter(), 2 * std::numbers::pi, 1e-10);
    EXPECT_NEAR(c2.perimeter(), 5 * std::numbers::pi, 1e-10);
    EXPECT_NEAR(c3.perimeter(), 20 * std::numbers::pi, 1e-10);
}

TEST(CircleTest, AreaCalculation) {
    Circle c1(1);
    Circle c2(2.5);
    Circle c3(10);

    EXPECT_NEAR(c1.area(), std::numbers::pi, 1e-10);
    EXPECT_NEAR(c2.area(), 6.25 * std::numbers::pi, 1e-10);
    EXPECT_NEAR(c3.area(), 100 * std::numbers::pi, 1e-10);
}

TEST(CircleTest, UnitCircle) {
    Circle c(1);
    EXPECT_NEAR(c.perimeter(), 2 * std::numbers::pi, 1e-10);
    EXPECT_NEAR(c.area(), std::numbers::pi, 1e-10);
}

TEST(ShapeComparisonTest, SameAreaDifferentShapes) {
    Square square(2);
    Circle circle(std::sqrt(4 / std::numbers::pi));

    EXPECT_NEAR(square.area(), circle.area(), 1e-10);
    EXPECT_NE(square.perimeter(), circle.perimeter());
}

TEST(ShapeComparisonTest, LargeValuesConsistency) {
    Triangle triangle(300, 400, 500);
    Square square(100);
    Circle circle(50);

    EXPECT_DOUBLE_EQ(triangle.area(), 60000.0);
    EXPECT_DOUBLE_EQ(square.area(), 10000.0);
    EXPECT_NEAR(circle.area(), 2500 * std::numbers::pi, 1e-10);
}

TEST(ShapeComparisonTest, PerimeterAreaRelationship) {
    Square square(5);
    EXPECT_DOUBLE_EQ(square.perimeter(), 20.0);
    EXPECT_DOUBLE_EQ(square.area(), 25.0);

    Circle circle(5);
    EXPECT_NEAR(circle.perimeter(), 10 * std::numbers::pi, 1e-10);
    EXPECT_NEAR(circle.area(), 25 * std::numbers::pi, 1e-10);
}

TEST(ShapeEdgeCases, VerySmallValues) {
    Triangle small_triangle(1e-10, 1e-10, 1e-10);
    Square small_square(1e-10);
    Circle small_circle(1e-10);

    EXPECT_GT(small_triangle.area(), 0);
    EXPECT_GT(small_square.area(), 0);
    EXPECT_GT(small_circle.area(), 0);
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
