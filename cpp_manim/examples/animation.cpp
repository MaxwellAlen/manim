#include "manimcpp/manimcpp.h"

class AnimationScene : public manim::Scene {
public:
    void construct() override {
        manim::Circle* circle = new manim::Circle(1.0f, manim::BLUE);
        manim::Square* square = new manim::Square(1.0f, manim::RED);
        
        add(circle);
        wait(0.5f);
        
        play(new manim::GrowFromCenter(circle, 1.0f));
        wait(0.5f);
        
        circle->shift(manim::LEFT * 2.0f);
        add(square);
        
        play(new manim::FadeIn(square, 1.0f));
        wait(0.5f);
        
        play(new manim::Rotate(square, manim::PI, manim::OUT, 2.0f));
        wait(0.5f);
        
        play(new manim::ScaleInPlace(circle, 2.0f, 1.0f));
        wait(0.5f);
        
        play(new manim::FadeOut(circle, 1.0f));
        play(new manim::FadeOut(square, 1.0f));
    }
};

int main() {
    AnimationScene scene;
    scene.run();
    return 0;
}