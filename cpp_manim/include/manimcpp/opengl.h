#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace manim {

class OpenGLContext {
public:
    OpenGLContext();
    ~OpenGLContext();
    
    void init();
    void enable_blend();
    void enable_depth_test();
    void set_clear_color(float r, float g, float b, float a);
    void clear();
    
    GLuint create_shader(GLenum type, const std::string& source);
    GLuint create_program(const std::string& vertex_source, const std::string& fragment_source);
    
    GLuint create_buffer(GLenum target, const void* data, size_t size, GLenum usage);
    GLuint create_vertex_array();
    
    void use_program(GLuint program);
    void bind_vertex_array(GLuint vao);
    void bind_buffer(GLenum target, GLuint buffer);
    
    void set_uniform(GLuint program, const std::string& name, float value);
    void set_uniform(GLuint program, const std::string& name, const glm::vec3& value);
    void set_uniform(GLuint program, const std::string& name, const glm::mat4& value);
    
    GLuint get_current_program() const { return current_program; }
    
private:
    GLuint current_program = 0;
};

class OpenGLWindow {
public:
    OpenGLWindow(int width, int height, const std::string& title = "ManimCPP");
    ~OpenGLWindow();
    
    void init();
    void make_context_current();
    void swap_buffers();
    void poll_events();
    bool should_close() const;
    void set_should_close(bool value);
    
    void resize(int width, int height);
    
    int get_width() const { return width; }
    int get_height() const { return height; }
    
    GLFWwindow* get_glfw_window() const { return window; }
    
private:
    GLFWwindow* window = nullptr;
    int width, height;
    std::string title;
};

class ShaderWrapper {
public:
    ShaderWrapper(const std::string& vertex_path, const std::string& fragment_path);
    ~ShaderWrapper();
    
    void init();
    void bind();
    void unbind();
    
    GLuint get_program() const { return program; }
    
    void set_uniform(const std::string& name, float value);
    void set_uniform(const std::string& name, const glm::vec3& value);
    void set_uniform(const std::string& name, const glm::mat4& value);
    
private:
    std::string vertex_path;
    std::string fragment_path;
    GLuint program = 0;
};

} // namespace manim