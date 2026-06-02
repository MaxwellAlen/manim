# ManimCPP

A C++ port of the ManimGL mathematical animation library.

## Features

- Core Mobject system for managing mathematical objects
- Scene management with animation support
- Camera system for rendering
- Geometry primitives (Circle, Square, Triangle, etc.)
- Animation system with various animation types
- OpenGL rendering backend

## Dependencies

- CMake 3.20+
- GLM (OpenGL Mathematics)
- GLFW 3
- GLEW
- fmt
- spdlog
- GTest (for tests)

## Building

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

```cpp
#include "manimcpp/manimcpp.h"

class MyScene : public manim::Scene {
public:
    void construct() override {
        manim::Circle* circle = new manim::Circle(1.0f, manim::BLUE);
        add(circle);
        play(new manim::GrowFromCenter(circle, 1.0f));
        wait(2.0f);
    }
};

int main() {
    MyScene scene;
    scene.run();
    return 0;
}
```

## License

MIT License