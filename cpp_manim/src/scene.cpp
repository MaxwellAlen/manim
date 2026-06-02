#include "manimcpp/scene.h"
#include "manimcpp/camera.h"
#include "manimcpp/animation.h"
#include "manimcpp/opengl.h"

namespace manim {

Scene::Scene(bool skip_animations, bool always_update_mobjects,
             int start_at_animation_number, int end_at_animation_number,
             bool show_animation_progress, float default_wait_time)
    : skip_animations(skip_animations), always_update_mobjects(always_update_mobjects),
      start_at_animation_number(start_at_animation_number),
      end_at_animation_number(end_at_animation_number),
      show_animation_progress(show_animation_progress),
      default_wait_time(default_wait_time) {
    camera = std::make_unique<Camera>();
    window = std::make_unique<OpenGLWindow>(
        camera->get_pixel_width(), camera->get_pixel_height(), "ManimCPP"
    );
}

Scene::~Scene() {
    for (auto mobj : mobjects) {
        delete mobj;
    }
}

void Scene::run() {
    setup();
    
    // Initialize window and OpenGL
    window->init();
    window->set_scene(this);
    camera->set_window(window.get());
    camera->init_context();
    
    construct();
    
    // Main loop
    float dt = 1.0f / camera->get_fps();
    while (!window->should_close()) {
        window->poll_events();
        update_frame(dt, true);
        window->swap_buffers();
    }
    
    tear_down();
}

// Default event handlers
void Scene::on_mouse_motion(double x, double y) {
    mouse_point = camera->pixel_coords_to_space_coords(x, y);
}

void Scene::on_mouse_press(double x, double y, int button, int mods) {
}

void Scene::on_mouse_release(double x, double y, int button, int mods) {
}

void Scene::on_key_press(int key, int mods) {
}

void Scene::on_key_release(int key, int mods) {
}

void Scene::on_resize(int width, int height) {
}

void Scene::setup() {
}

void Scene::construct() {
}

void Scene::tear_down() {
}

void Scene::add(Mobject* mobject) {
    if (std::find(mobjects.begin(), mobjects.end(), mobject) != mobjects.end()) {
        return;
    }
    mobjects.push_back(mobject);
    assemble_render_groups();
}

void Scene::remove(Mobject* mobject) {
    auto it = std::find(mobjects.begin(), mobjects.end(), mobject);
    if (it != mobjects.end()) {
        mobjects.erase(it);
        delete mobject;
        assemble_render_groups();
    }
}

void Scene::clear() {
    for (auto mobj : mobjects) {
        delete mobj;
    }
    mobjects.clear();
    assemble_render_groups();
}

void Scene::replace(Mobject* old_mobject, Mobject* new_mobject) {
    auto it = std::find(mobjects.begin(), mobjects.end(), old_mobject);
    if (it != mobjects.end()) {
        *it = new_mobject;
        delete old_mobject;
        assemble_render_groups();
    }
}

void Scene::bring_to_front(Mobject* mobject) {
    remove(mobject);
    mobjects.push_back(mobject);
    assemble_render_groups();
}

void Scene::bring_to_back(Mobject* mobject) {
    remove(mobject);
    mobjects.insert(mobjects.begin(), mobject);
    assemble_render_groups();
}

void Scene::play(Animation* animation) {
    play({animation});
}

void Scene::play(const std::vector<Animation*>& animations) {
    if (animations.empty()) return;
    
    pre_play();
    
    for (auto anim : animations) {
        anim->begin();
    }
    
    float run_time = 0.0f;
    for (auto anim : animations) {
        run_time = std::max(run_time, anim->get_run_time());
    }
    
    int num_frames = static_cast<int>(run_time * camera->get_fps());
    float dt = 1.0f / camera->get_fps();
    
    for (int i = 0; i <= num_frames; ++i) {
        float alpha = static_cast<float>(i) / num_frames;
        
        for (auto anim : animations) {
            anim->update_mobjects(dt);
            anim->interpolate(alpha);
        }
        
        update_frame(dt);
    }
    
    for (auto anim : animations) {
        anim->finish();
        anim->clean_up_from_scene(this);
    }
    
    post_play();
}

void Scene::wait(float duration) {
    if (duration < 0.0f) {
        duration = default_wait_time;
    }
    
    pre_play();
    
    int num_frames = static_cast<int>(duration * camera->get_fps());
    float dt = 1.0f / camera->get_fps();
    
    for (int i = 0; i < num_frames; ++i) {
        update_frame(dt);
    }
    
    post_play();
}

void Scene::wait_until(std::function<bool()> stop_condition, float max_time) {
    pre_play();
    
    float elapsed = 0.0f;
    float dt = 1.0f / camera->get_fps();
    
    while (elapsed < max_time) {
        if (stop_condition()) {
            break;
        }
        update_frame(dt);
        elapsed += dt;
    }
    
    post_play();
}

void Scene::set_background_color(const Color& color, float opacity) {
    camera->set_background_color(color, opacity);
}

void Scene::save_state() {
}

void Scene::undo() {
}

void Scene::redo() {
}

void Scene::update_mobjects(float dt) {
    for (auto mobj : mobjects) {
        mobj->update(dt);
    }
}

void Scene::update_frame(float dt, bool force_draw) {
    increment_time(dt);
    update_mobjects(dt);
    
    if (!skip_animations || force_draw) {
        camera->capture(mobjects);
    }
}

void Scene::assemble_render_groups() {
    render_groups.clear();
    if (mobjects.empty()) return;
    
    render_groups.push_back(new Group());
    render_groups[0]->add(mobjects[0]);
    
    for (size_t i = 1; i < mobjects.size(); ++i) {
        render_groups.push_back(new Group());
        render_groups.back()->add(mobjects[i]);
    }
}

void Scene::pre_play() {
    num_plays++;
}

void Scene::post_play() {
}

ThreeDScene::ThreeDScene() : Scene() {
    camera = std::make_unique<ThreeDCamera>();
}

void ThreeDScene::add(Mobject* mobject, bool set_depth_test) {
    if (set_depth_test) {
        mobject->depth_test = true;
    }
    Scene::add(mobject);
}

// InteractiveScene implementation
InteractiveScene::InteractiveScene(bool skip_animations, bool always_update_mobjects,
                                   int start_at_animation_number, int end_at_animation_number,
                                   bool show_animation_progress, float default_wait_time)
    : Scene(skip_animations, always_update_mobjects, start_at_animation_number,
            end_at_animation_number, show_animation_progress, default_wait_time) {
}

void InteractiveScene::setup() {
    // Base setup
}

void InteractiveScene::on_mouse_motion(double x, double y) {
    Scene::on_mouse_motion(x, y);
    if (is_grabbing && !selected_mobjects.empty()) {
        for (auto mobj : selected_mobjects) {
            mobj->move_to(mouse_point);
        }
    }
}

void InteractiveScene::on_mouse_press(double x, double y, int button, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        // Try to select a mobject
        for (auto it = mobjects.rbegin(); it != mobjects.rend(); ++it) {
            Mobject* mobj = *it;
            // Simple check: is mouse point within mobject's bounding box?
            // For simplicity, we'll just select the topmost mobject
            selected_mobjects.clear();
            selected_mobjects.push_back(mobj);
            is_grabbing = true;
            break;
        }
    }
}

void InteractiveScene::on_mouse_release(double x, double y, int button, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        is_grabbing = false;
    }
}

void InteractiveScene::on_key_press(int key, int mods) {
    // Example: press 'c' to clear selection
    if (key == GLFW_KEY_C) {
        selected_mobjects.clear();
    }
    // Example: press 'escape' to close window
    if (key == GLFW_KEY_ESCAPE) {
        window->set_should_close(true);
    }
}

void InteractiveScene::on_key_release(int key, int mods) {
}

} // namespace manim