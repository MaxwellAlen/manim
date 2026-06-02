#include "manimcpp/utils.h"

namespace manim {

float get_norm(const glm::vec3& v) {
    return glm::length(v);
}

float get_dist(const glm::vec3& v1, const glm::vec3& v2) {
    return glm::distance(v1, v2);
}

glm::vec3 normalize(const glm::vec3& v, const glm::vec3& fallback) {
    float norm = glm::length(v);
    if (norm > 0.0001f) {
        return v / norm;
    }
    return fallback;
}

glm::quat quaternion_from_angle_axis(float angle, const glm::vec3& axis) {
    return glm::angleAxis(angle, normalize(axis));
}

glm::mat4 rotation_matrix(float angle, const glm::vec3& axis) {
    return glm::rotate(glm::mat4(1.0f), angle, normalize(axis));
}

glm::mat4 rotation_matrix_transpose(float angle, const glm::vec3& axis) {
    return glm::transpose(rotation_matrix(angle, axis));
}

glm::vec3 rotate_vector(const glm::vec3& vector, float angle, const glm::vec3& axis) {
    return glm::rotate(glm::mat4(1.0f), angle, axis) * glm::vec4(vector, 1.0f);
}

float angle_of_vector(const glm::vec2& vector) {
    return atan2(vector.y, vector.x);
}

float angle_of_vector(const glm::vec3& vector) {
    return atan2(vector.y, vector.x);
}

float angle_between_vectors(const glm::vec3& v1, const glm::vec3& v2) {
    float dot = glm::dot(v1, v2);
    float norms = glm::length(v1) * glm::length(v2);
    if (norms < 0.0001f) return 0.0f;
    float cos_angle = dot / norms;
    return acos(glm::clamp(cos_angle, -1.0f, 1.0f));
}

glm::mat4 z_to_vector(const glm::vec3& vector) {
    glm::vec3 z_axis(0.0f, 0.0f, 1.0f);
    glm::vec3 axis = glm::cross(z_axis, vector);
    float angle = angle_between_vectors(z_axis, vector);
    if (glm::length(axis) < 0.0001f) {
        axis = glm::vec3(1.0f, 0.0f, 0.0f);
    }
    return rotation_matrix(angle, axis);
}

glm::vec3 midpoint(const glm::vec3& p1, const glm::vec3& p2) {
    return (p1 + p2) / 2.0f;
}

float interpolate(float start, float end, float alpha) {
    return start + (end - start) * alpha;
}

glm::vec3 interpolate(const glm::vec3& start, const glm::vec3& end, float alpha) {
    return start + (end - start) * alpha;
}

Color interpolate(const Color& start, const Color& end, float alpha) {
    return Color(
        interpolate(start.r, end.r, alpha),
        interpolate(start.g, end.g, alpha),
        interpolate(start.b, end.b, alpha),
        interpolate(start.a, end.a, alpha)
    );
}

float smooth(float alpha) {
    if (alpha <= 0.0f) return 0.0f;
    if (alpha >= 1.0f) return 1.0f;
    float t = alpha;
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float linear(float alpha) {
    return alpha;
}

float ease_in(float alpha) {
    return alpha * alpha;
}

float ease_out(float alpha) {
    return 1.0f - (1.0f - alpha) * (1.0f - alpha);
}

float ease_in_out(float alpha) {
    if (alpha < 0.5f) {
        return 2.0f * alpha * alpha;
    }
    return 1.0f - pow(-2.0f * alpha + 2.0f, 2.0f) / 2.0f;
}

} // namespace manim