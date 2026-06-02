#pragma once

#include <vector>

#include "manimcpp/mobject.h"

namespace manim {

class Point : public Mobject {
public:
    Point(const glm::vec3& position = ORIGIN, const Color& color = DEFAULT_MOBJECT_COLOR,
          float opacity = 1.0f);
    
    void init_points() override;
};

class Line : public Mobject {
public:
    Line(const glm::vec3& start = ORIGIN, const glm::vec3& end = RIGHT,
         const Color& color = DEFAULT_MOBJECT_COLOR, float opacity = 1.0f,
         float stroke_width = 4.0f);
    
    void init_points() override;
    
    glm::vec3 get_start() const;
    glm::vec3 get_end() const;
    
    void put_start_and_end_on(const glm::vec3& start, const glm::vec3& end);
    
protected:
    glm::vec3 start, end;
    float stroke_width;
};

class Circle : public Mobject {
public:
    Circle(float radius = 1.0f, const Color& color = DEFAULT_MOBJECT_COLOR,
           float opacity = 1.0f, int num_points = 100);
    
    void init_points() override;
    
protected:
    float radius;
    int num_points;
};

class Square : public Mobject {
public:
    Square(float side_length = 2.0f, const Color& color = DEFAULT_MOBJECT_COLOR,
           float opacity = 1.0f);
    
    void init_points() override;
    
protected:
    float side_length;
};

class Triangle : public Mobject {
public:
    Triangle(float side_length = 2.0f, const Color& color = DEFAULT_MOBJECT_COLOR,
             float opacity = 1.0f);
    
    void init_points() override;
    
protected:
    float side_length;
};

class Rectangle : public Mobject {
public:
    Rectangle(float width = 4.0f, float height = 2.0f,
              const Color& color = DEFAULT_MOBJECT_COLOR, float opacity = 1.0f);
    
    void init_points() override;
    
protected:
    float width, height;
};

class Arrow : public Mobject {
public:
    Arrow(const glm::vec3& start = ORIGIN, const glm::vec3& end = RIGHT,
          const Color& color = DEFAULT_MOBJECT_COLOR, float opacity = 1.0f,
          float stroke_width = 4.0f, float tip_length = 0.3f);
    
    void init_points() override;
    
protected:
    glm::vec3 start, end;
    float stroke_width;
    float tip_length;
};

class RegularPolygon : public Mobject {
public:
    RegularPolygon(int num_sides = 6, float radius = 1.0f,
                   const Color& color = DEFAULT_MOBJECT_COLOR, float opacity = 1.0f);
    
    void init_points() override;
    
protected:
    int num_sides;
    float radius;
};

} // namespace manim