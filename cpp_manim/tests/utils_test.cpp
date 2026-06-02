#include <gtest/gtest.h>
#include "manimcpp/utils.h"

TEST(UtilsTest, GetNorm) {
    glm::vec3 v(3.0f, 4.0f, 0.0f);
    EXPECT_FLOAT_EQ(manim::get_norm(v), 5.0f);
}

TEST(UtilsTest, Normalize) {
    glm::vec3 v(3.0f, 4.0f, 0.0f);
    glm::vec3 normalized = manim::normalize(v);
    
    EXPECT_FLOAT_EQ(manim::get_norm(normalized), 1.0f);
    EXPECT_FLOAT_EQ(normalized.x, 0.6f);
    EXPECT_FLOAT_EQ(normalized.y, 0.8f);
}

TEST(UtilsTest, Distance) {
    glm::vec3 v1(0.0f, 0.0f, 0.0f);
    glm::vec3 v2(3.0f, 4.0f, 0.0f);
    
    EXPECT_FLOAT_EQ(manim::get_dist(v1, v2), 5.0f);
}

TEST(UtilsTest, Interpolate) {
    float result = manim::interpolate(0.0f, 10.0f, 0.5f);
    EXPECT_FLOAT_EQ(result, 5.0f);
    
    glm::vec3 v1(0.0f, 0.0f, 0.0f);
    glm::vec3 v2(2.0f, 4.0f, 6.0f);
    glm::vec3 v_result = manim::interpolate(v1, v2, 0.5f);
    
    EXPECT_FLOAT_EQ(v_result.x, 1.0f);
    EXPECT_FLOAT_EQ(v_result.y, 2.0f);
    EXPECT_FLOAT_EQ(v_result.z, 3.0f);
}

TEST(UtilsTest, RateFunctions) {
    EXPECT_FLOAT_EQ(manim::linear(0.0f), 0.0f);
    EXPECT_FLOAT_EQ(manim::linear(1.0f), 1.0f);
    EXPECT_FLOAT_EQ(manim::linear(0.5f), 0.5f);
    
    EXPECT_FLOAT_EQ(manim::smooth(0.0f), 0.0f);
    EXPECT_FLOAT_EQ(manim::smooth(1.0f), 1.0f);
    
    EXPECT_FLOAT_EQ(manim::ease_in(0.0f), 0.0f);
    EXPECT_FLOAT_EQ(manim::ease_in(1.0f), 1.0f);
    EXPECT_FLOAT_EQ(manim::ease_in(0.5f), 0.25f);
    
    EXPECT_FLOAT_EQ(manim::ease_out(0.0f), 0.0f);
    EXPECT_FLOAT_EQ(manim::ease_out(1.0f), 1.0f);
    EXPECT_FLOAT_EQ(manim::ease_out(0.5f), 0.75f);
}

TEST(UtilsTest, RotationMatrix) {
    glm::mat4 rot = manim::rotation_matrix(manim::PI, manim::Z_AXIS);
    glm::vec4 v(1.0f, 0.0f, 0.0f, 1.0f);
    
    glm::vec4 result = rot * v;
    
    EXPECT_NEAR(result.x, -1.0f, 0.001f);
    EXPECT_NEAR(result.y, 0.0f, 0.001f);
}