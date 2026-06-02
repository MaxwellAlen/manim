#include "manimcpp/geometry.h"

namespace manim {

Point::Point(const glm::vec3& position, const Color& color, float opacity)
    : Mobject(color, opacity) {
    init_points();
    move_to(position);
}

void Point::init_points() {
    set_points({glm::vec3(0.0f)});
}

Line::Line(const glm::vec3& start, const glm::vec3& end,
           const Color& color, float opacity, float stroke_width)
    : Mobject(color, opacity), start(start), end(end), stroke_width(stroke_width) {
    init_points();
}

void Line::init_points() {
    set_points({start, end});
}

glm::vec3 Line::get_start() const {
    return start;
}

glm::vec3 Line::get_end() const {
    return end;
}

void Line::put_start_and_end_on(const glm::vec3& new_start, const glm::vec3& new_end) {
    start = new_start;
    end = new_end;
    init_points();
}

Circle::Circle(float radius, const Color& color, float opacity, int num_points)
    : Mobject(color, opacity), radius(radius), num_points(num_points) {
    init_points();
}

void Circle::init_points() {
    std::vector<glm::vec3> points;
    points.reserve(num_points);
    
    for (int i = 0; i < num_points; ++i) {
        float angle = static_cast<float>(i) / num_points * TAU;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        points.emplace_back(x, y, 0.0f);
    }
    
    set_points(points);
}

Square::Square(float side_length, const Color& color, float opacity)
    : Mobject(color, opacity), side_length(side_length) {
    init_points();
}

void Square::init_points() {
    float half = side_length / 2.0f;
    std::vector<glm::vec3> points = {
        {-half, -half, 0.0f},
        {half, -half, 0.0f},
        {half, half, 0.0f},
        {-half, half, 0.0f},
        {-half, -half, 0.0f}
    };
    set_points(points);
}

Triangle::Triangle(float side_length, const Color& color, float opacity)
    : Mobject(color, opacity), side_length(side_length) {
    init_points();
}

void Triangle::init_points() {
    float height = side_length * sqrt(3.0f) / 2.0f;
    std::vector<glm::vec3> points = {
        {0.0f, height / 3.0f, 0.0f},
        {-side_length / 2.0f, -height / 3.0f, 0.0f},
        {side_length / 2.0f, -height / 3.0f, 0.0f},
        {0.0f, height / 3.0f, 0.0f}
    };
    set_points(points);
}

Rectangle::Rectangle(float width, float height, const Color& color, float opacity)
    : Mobject(color, opacity), width(width), height(height) {
    init_points();
}

void Rectangle::init_points() {
    float half_width = width / 2.0f;
    float half_height = height / 2.0f;
    std::vector<glm::vec3> points = {
        {-half_width, -half_height, 0.0f},
        {half_width, -half_height, 0.0f},
        {half_width, half_height, 0.0f},
        {-half_width, half_height, 0.0f},
        {-half_width, -half_height, 0.0f}
    };
    set_points(points);
}

Arrow::Arrow(const glm::vec3& start, const glm::vec3& end,
             const Color& color, float opacity, float stroke_width, float tip_length)
    : Mobject(color, opacity), start(start), end(end),
      stroke_width(stroke_width), tip_length(tip_length) {
    init_points();
}

void Arrow::init_points() {
    glm::vec3 direction = normalize(end - start);
    glm::vec3 perpendicular = glm::vec3(-direction.y, direction.x, 0.0f);
    
    glm::vec3 tip_base = end - direction * tip_length;
    glm::vec3 arrow_left = tip_base - direction * tip_length / 3.0f + perpendicular * tip_length / 2.0f;
    glm::vec3 arrow_right = tip_base - direction * tip_length / 3.0f - perpendicular * tip_length / 2.0f;
    
    std::vector<glm::vec3> points = {
        start,
        tip_base,
        end,
        tip_base,
        arrow_left,
        tip_base,
        arrow_right
    };
    set_points(points);
}

RegularPolygon::RegularPolygon(int num_sides, float radius,
                               const Color& color, float opacity)
    : Mobject(color, opacity), num_sides(num_sides), radius(radius) {
    init_points();
}

void RegularPolygon::init_points() {
    std::vector<glm::vec3> points;
    points.reserve(num_sides + 1);
    
    for (int i = 0; i < num_sides; ++i) {
        float angle = static_cast<float>(i) / num_sides * TAU - PI / 2.0f;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        points.emplace_back(x, y, 0.0f);
    }
    points.push_back(points[0]);
    
    set_points(points);
}

} // namespace manim