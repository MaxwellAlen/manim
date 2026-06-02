#include "manimcpp/camera.h"
#include "manimcpp/opengl.h"

namespace manim {

Camera::Camera(int resolution_x, int resolution_y, int fps,
               const Color& background_color, float background_opacity,
               float max_allowable_norm, int samples)
    : resolution_x(resolution_x), resolution_y(resolution_y), fps(fps),
      max_allowable_norm(max_allowable_norm), samples(samples) {
    background_rgba = {background_color.r, background_color.g, background_color.b, background_opacity};
    init_context();
    init_frame();
}

Camera::~Camera() {
}

void Camera::init_context() {
    ctx = std::make_unique<OpenGLContext>();
    ctx->init();
}

void Camera::init_frame() {
    frame = std::make_unique<class CameraFrame>();
}

void Camera::init_fbo() {
}

void Camera::set_window(class OpenGLWindow* window) {
    this->window = window;
}

void Camera::capture(const std::vector<Mobject*>& mobjects) {
    clear();
    refresh_uniforms();
    
    for (auto mobj : mobjects) {
        mobj->render(*ctx, uniforms);
    }
}

void Camera::clear() {
    ctx->set_clear_color(background_rgba[0], background_rgba[1], background_rgba[2], background_rgba[3]);
    ctx->clear();
}

void Camera::refresh_uniforms() {
    glm::mat4 view_matrix = frame->get_view_matrix();
    glm::vec3 light_pos = glm::vec3(-10.0f, 10.0f, 10.0f);
    glm::vec3 cam_pos = frame->get_implied_camera_location();
    
    uniforms["frame_scale"] = frame->get_scale();
}

float Camera::get_pixel_size() const {
    return frame->get_width() / static_cast<float>(resolution_x);
}

std::pair<int, int> Camera::get_pixel_shape() const {
    return {resolution_x, resolution_y};
}

int Camera::get_pixel_width() const {
    return resolution_x;
}

int Camera::get_pixel_height() const {
    return resolution_y;
}

float Camera::get_aspect_ratio() const {
    return static_cast<float>(resolution_x) / static_cast<float>(resolution_y);
}

float Camera::get_frame_height() const {
    return frame->get_height();
}

float Camera::get_frame_width() const {
    return frame->get_width();
}

glm::vec2 Camera::get_frame_shape() const {
    return {get_frame_width(), get_frame_height()};
}

glm::vec3 Camera::get_frame_center() const {
    return frame->get_center();
}

glm::vec3 Camera::get_location() const {
    return frame->get_implied_camera_location();
}

void Camera::resize_frame_shape(bool fixed_dimension) {
    float frame_height = get_frame_height();
    float frame_width = get_frame_width();
    float aspect_ratio = get_aspect_ratio();
    
    if (!fixed_dimension) {
        frame_height = frame_width / aspect_ratio;
    } else {
        frame_width = aspect_ratio * frame_height;
    }
    
    frame->set_height(frame_height, true);
    frame->set_width(frame_width, true);
}

std::vector<uint8_t> Camera::get_pixel_array() {
    return {};
}

void Camera::set_background_color(const Color& color, float opacity) {
    background_rgba = {color.r, color.g, color.b, opacity};
}

ThreeDCamera::ThreeDCamera(int samples) : Camera(1920, 1080, DEFAULT_FPS, BLACK, 1.0f, FRAME_WIDTH, samples) {
}

} // namespace manim