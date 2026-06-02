#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "constants.h"

namespace manim {

float get_norm(const glm::vec3& v);
float get_dist(const glm::vec3& v1, const glm::vec3& v2);

glm::vec3 normalize(const glm::vec3& v, const glm::vec3& fallback = glm::vec3(0.0f));

glm::quat quaternion_from_angle_axis(float angle, const glm::vec3& axis);
glm::mat4 rotation_matrix(float angle, const glm::vec3& axis);
glm::mat4 rotation_matrix_transpose(float angle, const glm::vec3& axis);

glm::vec3 rotate_vector(const glm::vec3& vector, float angle, const glm::vec3& axis = glm::vec3(0.0f, 0.0f, 1.0f));

float angle_of_vector(const glm::vec2& vector);
float angle_of_vector(const glm::vec3& vector);

float angle_between_vectors(const glm::vec3& v1, const glm::vec3& v2);

glm::mat4 z_to_vector(const glm::vec3& vector);

glm::vec3 midpoint(const glm::vec3& p1, const glm::vec3& p2);

float interpolate(float start, float end, float alpha);

glm::vec3 interpolate(const glm::vec3& start, const glm::vec3& end, float alpha);

Color interpolate(const Color& start, const Color& end, float alpha);

float smooth(float alpha);
float linear(float alpha);
float ease_in(float alpha);
float ease_out(float alpha);
float ease_in_out(float alpha);

} // namespace manim