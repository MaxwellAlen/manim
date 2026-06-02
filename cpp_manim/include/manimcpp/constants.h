#pragma once

#include <glm/glm.hpp>

namespace manim {

constexpr float PI = 3.14159265358979323846f;
constexpr float TAU = 2.0f * PI;
constexpr float DEG = TAU / 360.0f;
constexpr float RADIANS = 1.0f;

constexpr float FRAME_HEIGHT = 8.0f;
constexpr float FRAME_WIDTH = 14.2222222222f;
constexpr float FRAME_Y_RADIUS = FRAME_HEIGHT / 2.0f;
constexpr float FRAME_X_RADIUS = FRAME_WIDTH / 2.0f;

constexpr float SMALL_BUFF = 0.1f;
constexpr float MED_SMALL_BUFF = 0.25f;
constexpr float MED_LARGE_BUFF = 0.5f;
constexpr float LARGE_BUFF = 1.0f;
constexpr float DEFAULT_MOBJECT_TO_EDGE_BUFF = 0.5f;
constexpr float DEFAULT_MOBJECT_TO_MOBJECT_BUFF = 0.25f;

const glm::vec3 ORIGIN = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 DOWN = glm::vec3(0.0f, -1.0f, 0.0f);
const glm::vec3 RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 LEFT = glm::vec3(-1.0f, 0.0f, 0.0f);
const glm::vec3 IN = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 OUT = glm::vec3(0.0f, 0.0f, 1.0f);
const glm::vec3 X_AXIS = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 Y_AXIS = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 Z_AXIS = glm::vec3(0.0f, 0.0f, 1.0f);

const glm::vec3 UL = UP + LEFT;
const glm::vec3 UR = UP + RIGHT;
const glm::vec3 DL = DOWN + LEFT;
const glm::vec3 DR = DOWN + RIGHT;

const glm::vec3 TOP = FRAME_Y_RADIUS * UP;
const glm::vec3 BOTTOM = FRAME_Y_RADIUS * DOWN;
const glm::vec3 LEFT_SIDE = FRAME_X_RADIUS * LEFT;
const glm::vec3 RIGHT_SIDE = FRAME_X_RADIUS * RIGHT;

constexpr int DEFAULT_FPS = 30;
constexpr int DEFAULT_SAMPLES = 0;

struct Color {
    float r, g, b, a;
    
    Color() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
    Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}
    
    static const Color BLACK;
    static const Color WHITE;
    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;
    static const Color YELLOW;
    static const Color PURPLE;
    static const Color ORANGE;
    static const Color TEAL;
    static const Color PINK;
    static const Color GREY;
};

extern const Color BLACK;
extern const Color WHITE;
extern const Color RED;
extern const Color GREEN;
extern const Color BLUE;
extern const Color YELLOW;
extern const Color PURPLE;
extern const Color ORANGE;
extern const Color TEAL;
extern const Color PINK;
extern const Color GREY;

extern const Color DEFAULT_MOBJECT_COLOR;
extern const Color DEFAULT_LIGHT_COLOR;

} // namespace manim