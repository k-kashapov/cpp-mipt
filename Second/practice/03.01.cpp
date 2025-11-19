#include <algorithm>
#include <climits>
#include <gtest/gtest.h>
#include <vector>

struct Rectangle {
    int left;
    int top;
    int right;
    int bottom;

    Rectangle(int l, int t, int r, int b) : left(l), top(t), right(r), bottom(b) {}

    int width() const { return std::max(0, right - left); }
    int height() const { return std::max(0, bottom - top); }
    int area() const { return width() * height(); }
};

Rectangle intersection(const Rectangle &a, const Rectangle &b) {
    int inter_left = std::max(a.left, b.left);
    int inter_top = std::max(a.top, b.top);
    int inter_right = std::min(a.right, b.right);
    int inter_bottom = std::min(a.bottom, b.bottom);

    if (inter_left >= inter_right || inter_top >= inter_bottom) {
        return Rectangle(0, 0, 0, 0);
    }

    return Rectangle(inter_left, inter_top, inter_right, inter_bottom);
}

int intersection_area(const std::vector<Rectangle> &rectangles) {
    if (rectangles.empty())
        return 0;

    Rectangle current = rectangles[0];
    for (size_t i = 1; i < rectangles.size(); ++i) {
        current = intersection(current, rectangles[i]);
        if (current.area() == 0)
            return 0;
    }

    return current.area();
}

Rectangle bounding_box(const std::vector<Rectangle> &rectangles) {
    if (rectangles.empty())
        return Rectangle(0, 0, 0, 0);

    int min_left = std::numeric_limits<int>::max();
    int min_top = std::numeric_limits<int>::max();
    int max_right = std::numeric_limits<int>::min();
    ;
    int max_bottom = std::numeric_limits<int>::min();
    ;

    for (const auto &rect : rectangles) {
        min_left = std::min(min_left, rect.left);
        min_top = std::min(min_top, rect.top);
        max_right = std::max(max_right, rect.right);
        max_bottom = std::max(max_bottom, rect.bottom);
    }

    return Rectangle(min_left, min_top, max_right, max_bottom);
}

TEST(RectangleTest, BasicProperties) {
    Rectangle rect(1, 2, 5, 6);
    EXPECT_EQ(rect.width(), 4);
    EXPECT_EQ(rect.height(), 4);
    EXPECT_EQ(rect.area(), 16);
}

TEST(RectangleTest, ZeroAreaRectangle) {
    Rectangle rect1(1, 1, 1, 5);
    Rectangle rect2(1, 1, 5, 1);
    EXPECT_EQ(rect1.area(), 0);
    EXPECT_EQ(rect2.area(), 0);
}

TEST(RectangleTest, IntersectionBasic) {
    Rectangle a(0, 0, 5, 5);
    Rectangle b(2, 2, 7, 7);
    Rectangle result = intersection(a, b);
    EXPECT_EQ(result.left, 2);
    EXPECT_EQ(result.top, 2);
    EXPECT_EQ(result.right, 5);
    EXPECT_EQ(result.bottom, 5);
    EXPECT_EQ(result.area(), 9);
}

TEST(RectangleTest, IntersectionNoOverlap) {
    Rectangle a(0, 0, 3, 3);
    Rectangle b(5, 5, 8, 8);
    Rectangle result = intersection(a, b);
    EXPECT_EQ(result.area(), 0);
}

TEST(RectangleTest, IntersectionTouchingEdges) {
    Rectangle a(0, 0, 3, 3);
    Rectangle b(3, 0, 6, 3);
    Rectangle result = intersection(a, b);
    EXPECT_EQ(result.area(), 0);
}

TEST(RectangleTest, IntersectionCompleteContainment) {
    Rectangle a(1, 1, 6, 6);
    Rectangle b(2, 2, 4, 4);
    Rectangle result = intersection(a, b);
    EXPECT_EQ(result.left, 2);
    EXPECT_EQ(result.top, 2);
    EXPECT_EQ(result.right, 4);
    EXPECT_EQ(result.bottom, 4);
    EXPECT_EQ(result.area(), 4);
}

TEST(RectangleTest, IntersectionAreaSingle) {
    std::vector<Rectangle> rects = {Rectangle(0, 0, 5, 5)};
    EXPECT_EQ(intersection_area(rects), 25);
}

TEST(RectangleTest, IntersectionAreaMultiple) {
    std::vector<Rectangle> rects = {Rectangle(0, 0, 5, 5), Rectangle(2, 2, 7, 7),
                                    Rectangle(3, 1, 6, 4)};
    EXPECT_EQ(intersection_area(rects), 4);
}

TEST(RectangleTest, IntersectionAreaEmpty) {
    std::vector<Rectangle> rects = {Rectangle(0, 0, 3, 3), Rectangle(5, 5, 8, 8)};
    EXPECT_EQ(intersection_area(rects), 0);
}

TEST(RectangleTest, IntersectionAreaEmptyVector) {
    std::vector<Rectangle> rects;
    EXPECT_EQ(intersection_area(rects), 0);
}

TEST(RectangleTest, BoundingBoxBasic) {
    std::vector<Rectangle> rects = {Rectangle(1, 1, 3, 3), Rectangle(2, 2, 5, 5),
                                    Rectangle(0, 0, 4, 2)};
    Rectangle result = bounding_box(rects);
    EXPECT_EQ(result.left, 0);
    EXPECT_EQ(result.top, 0);
    EXPECT_EQ(result.right, 5);
    EXPECT_EQ(result.bottom, 5);
}

TEST(RectangleTest, BoundingBoxSingle) {
    std::vector<Rectangle> rects = {Rectangle(2, 3, 7, 9)};
    Rectangle result = bounding_box(rects);
    EXPECT_EQ(result.left, 2);
    EXPECT_EQ(result.top, 3);
    EXPECT_EQ(result.right, 7);
    EXPECT_EQ(result.bottom, 9);
}

TEST(RectangleTest, BoundingBoxEmpty) {
    std::vector<Rectangle> rects;
    Rectangle result = bounding_box(rects);
    EXPECT_EQ(result.area(), 0);
}

TEST(RectangleTest, NegativeCoordinates) {
    Rectangle a(-5, -5, -1, -1);
    Rectangle b(-3, -3, 2, 2);
    Rectangle result = intersection(a, b);
    EXPECT_EQ(result.left, -3);
    EXPECT_EQ(result.top, -3);
    EXPECT_EQ(result.right, -1);
    EXPECT_EQ(result.bottom, -1);
    EXPECT_EQ(result.area(), 4);
}

TEST(RectangleTest, LargeCoordinates) {
    Rectangle a(0, 0, 1000, 1000);
    Rectangle b(500, 500, 1500, 1500);
    Rectangle result = intersection(a, b);
    EXPECT_EQ(result.area(), 250000);
}

TEST(RectangleTest, DegenerateRectangles) {
    Rectangle line_h(0, 0, 5, 0);
    Rectangle line_v(0, 0, 0, 5);
    Rectangle point(2, 2, 2, 2);

    EXPECT_EQ(line_h.area(), 0);
    EXPECT_EQ(line_v.area(), 0);
    EXPECT_EQ(point.area(), 0);
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}