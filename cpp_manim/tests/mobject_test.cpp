#include <gtest/gtest.h>
#include "manimcpp/mobject.h"
#include "manimcpp/geometry.h"

TEST(MobjectTest, Initialization) {
    manim::Mobject mobj;
    EXPECT_EQ(mobj.get_num_points(), 0);
    EXPECT_FALSE(mobj.has_points());
}

TEST(MobjectTest, AddSubmobject) {
    manim::Group group;
    manim::Circle* circle = new manim::Circle(1.0f);
    
    group.add(circle);
    
    EXPECT_EQ(group.size(), 1);
    EXPECT_TRUE(group[0] == circle);
}

TEST(MobjectTest, Transformations) {
    manim::Circle circle(1.0f);
    
    glm::vec3 initial_center = circle.get_center();
    
    circle.shift(manim::RIGHT * 2.0f);
    glm::vec3 new_center = circle.get_center();
    
    EXPECT_FLOAT_EQ(new_center.x, initial_center.x + 2.0f);
}

TEST(MobjectTest, ColorAndOpacity) {
    manim::Circle circle(1.0f, manim::RED);
    
    EXPECT_EQ(circle.get_color().r, manim::RED.r);
    EXPECT_EQ(circle.get_color().g, manim::RED.g);
    EXPECT_EQ(circle.get_color().b, manim::RED.b);
    
    circle.set_opacity(0.5f);
    EXPECT_FLOAT_EQ(circle.get_opacity(), 0.5f);
}

TEST(CircleTest, Initialization) {
    manim::Circle circle(2.0f);
    
    EXPECT_GT(circle.get_num_points(), 0);
    EXPECT_FLOAT_EQ(circle.get_width(), 4.0f);
}

TEST(SquareTest, Initialization) {
    manim::Square square(2.0f);
    
    EXPECT_GT(square.get_num_points(), 0);
    EXPECT_FLOAT_EQ(square.get_width(), 2.0f);
    EXPECT_FLOAT_EQ(square.get_height(), 2.0f);
}