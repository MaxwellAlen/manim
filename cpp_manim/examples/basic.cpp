#include "manimcpp/manimcpp.h"

class BasicScene : public manim::InteractiveScene {
public:
    void construct() override {
        manim::Circle* circle = new manim::Circle(1.0f, manim::BLUE);
        manim::Square* square = new manim::Square(1.0f, manim::RED);
        manim::Triangle* triangle = new manim::Triangle(1.0f, manim::GREEN);
        
        circle->shift(manim::LEFT * 3.0f);
        square->move_to(manim::ORIGIN);
        triangle->shift(manim::RIGHT * 3.0f);
        
        add(circle);
        add(square);
        add(triangle);
    }
};

int main() {
    BasicScene scene;
    scene.run();
    return 0;
}