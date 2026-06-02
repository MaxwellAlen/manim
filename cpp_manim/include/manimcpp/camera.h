#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

#include "manimcpp/constants.h"
#include "manimcpp/mobject.h"

namespace manim {

class OpenGLContext;

class CameraFrame : public Mobject {
public:
    CameraFrame() : Mobject() {}
    
    glm::mat4 get_view_matrix() const {
        return glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    
    glm::vec3 get_implied_camera_location() const {
        return glm::vec3(0.0f, 0.0f, 5.0f);
    }
    
    float get_scale() const {
        return 1.0f;
    }
    
    float get_width() const {
        return FRAME_WIDTH;
    }
    
    float get_height() const {
        return FRAME_HEIGHT;
    }
    
    glm::vec3 get_center() const {
        return ORIGIN;
    }
    
    void set_height(float height, bool stretch = false) {}
    void set_width(float width, bool stretch = false) {}
};

class Camera {
public:
    Camera(int resolution_x = 1920, int resolution_y = 1080, int fps = DEFAULT_FPS,
           const Color& background_color = BLACK, float background_opacity = 1.0f,
           float max_allowable_norm = FRAME_WIDTH, int samples = DEFAULT_SAMPLES);
    
    virtual ~Camera();
    
    void init_context();
    void init_frame();
    void init_fbo();
    
    void set_window(class OpenGLWindow* window);
    
    void capture(const std::vector<Mobject*>& mobjects);
    void clear();
    
    void refresh_uniforms();
    
    float get_pixel_size() const;
    std::pair<int, int> get_pixel_shape() const;
    int get_pixel_width() const;
    int get_pixel_height() const;
    float get_aspect_ratio() const;
    int get_fps() const { return fps; }
    
    float get_frame_height() const;
    float get_frame_width() const;
    glm::vec2 get_frame_shape() const;
    glm::vec3 get_frame_center() const;
    
    glm::vec3 get_location() const;
    
    void resize_frame_shape(bool fixed_dimension = false);
    
    // Pixel to space coordinate conversion
    glm::vec3 pixel_coords_to_space_coords(float x, float y, bool relative = false) const;
    
    std::vector<uint8_t> get_pixel_array();
    
    CameraFrame* get_frame() const { return frame.get(); }
    
    void set_background_color(const Color& color, float opacity);
    OpenGLContext* get_context() const { return ctx.get(); }
    
protected:
    class OpenGLWindow* window = nullptr;
    std::unique_ptr<OpenGLContext> ctx;
    std::unique_ptr<CameraFrame> frame;
    
    int resolution_x, resolution_y;
    int fps;
    float max_allowable_norm;
    int samples;
    
    std::vector<float> background_rgba;
    std::unordered_map<std::string, float> uniforms;
};

class ThreeDCamera : public Camera {
public:
    ThreeDCamera(int samples = 4);
};

} // namespace manim