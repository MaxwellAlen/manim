#pragma once

#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <unordered_map>
#include <queue>

#include "manimcpp/constants.h"
#include "manimcpp/mobject.h"

namespace manim {

class Camera;
class Animation;
class OpenGLWindow;

class Scene {
public:
    Scene(bool skip_animations = false, bool always_update_mobjects = false,
          int start_at_animation_number = -1, int end_at_animation_number = -1,
          bool show_animation_progress = false, float default_wait_time = 1.0f);
    
    virtual ~Scene();
    
    void run();
    virtual void setup();
    virtual void construct();
    virtual void tear_down();
    
    void add(Mobject* mobject);
    void remove(Mobject* mobject);
    void clear();
    void replace(Mobject* old_mobject, Mobject* new_mobject);
    
    void bring_to_front(Mobject* mobject);
    void bring_to_back(Mobject* mobject);
    
    void play(Animation* animation);
    void play(const std::vector<Animation*>& animations);
    
    void wait(float duration = -1.0f);
    void wait_until(std::function<bool()> stop_condition, float max_time = 60.0f);
    
    void set_background_color(const Color& color, float opacity = 1.0f);
    
    void save_state();
    void undo();
    void redo();
    
    Camera* get_camera() const { return camera.get(); }
    OpenGLWindow* get_window() const { return window.get(); }
    
    float get_time() const { return time; }
    void increment_time(float dt) { time += dt; }
    
    void update_mobjects(float dt);
    void update_frame(float dt = 0.0f, bool force_draw = false);
    
    // Event handlers
    virtual void on_mouse_motion(double x, double y);
    virtual void on_mouse_press(double x, double y, int button, int mods);
    virtual void on_mouse_release(double x, double y, int button, int mods);
    virtual void on_key_press(int key, int mods);
    virtual void on_key_release(int key, int mods);
    virtual void on_resize(int width, int height);
    
    glm::vec3 get_mouse_point() const { return mouse_point; }
    
protected:
    std::unique_ptr<Camera> camera;
    std::unique_ptr<OpenGLWindow> window;
    
    glm::vec3 mouse_point = glm::vec3(0.0f);
    
    std::vector<Mobject*> mobjects;
    std::vector<Mobject*> render_groups;
    
    float time = 0.0f;
    int num_plays = 0;
    
    bool skip_animations = false;
    bool always_update_mobjects = false;
    int start_at_animation_number = -1;
    int end_at_animation_number = -1;
    bool show_animation_progress = false;
    float default_wait_time = 1.0f;
    
    std::vector<std::function<void()>> undo_stack;
    std::vector<std::function<void()>> redo_stack;
    
    void assemble_render_groups();
    void pre_play();
    void post_play();
};

class ThreeDScene : public Scene {
public:
    ThreeDScene();
    
    void add(Mobject* mobject, bool set_depth_test = true);
};

class InteractiveScene : public Scene {
public:
    InteractiveScene(bool skip_animations = false, bool always_update_mobjects = false,
                     int start_at_animation_number = -1, int end_at_animation_number = -1,
                     bool show_animation_progress = false, float default_wait_time = 1.0f);
    
    void setup() override;
    
    // Event handling overrides
    void on_mouse_motion(double x, double y) override;
    void on_mouse_press(double x, double y, int button, int mods) override;
    void on_mouse_release(double x, double y, int button, int mods) override;
    void on_key_press(int key, int mods) override;
    void on_key_release(int key, int mods) override;
    
protected:
    bool is_grabbing = false;
    std::vector<Mobject*> selected_mobjects;
};

} // namespace manim