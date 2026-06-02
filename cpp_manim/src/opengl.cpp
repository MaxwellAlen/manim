#include "manimcpp/opengl.h"
#include "manimcpp/scene.h"
#include <stdexcept>
#include <fstream>
#include <sstream>

namespace manim {

OpenGLContext::OpenGLContext() {
}

OpenGLContext::~OpenGLContext() {
}

void OpenGLContext::init() {
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        throw std::runtime_error("Failed to initialize GLEW");
    }
}

void OpenGLContext::enable_blend() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLContext::enable_depth_test() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void OpenGLContext::set_clear_color(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void OpenGLContext::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLuint OpenGLContext::create_shader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(shader, 512, nullptr, info_log);
        throw std::runtime_error("Shader compilation failed: " + std::string(info_log));
    }
    
    return shader;
}

GLuint OpenGLContext::create_program(const std::string& vertex_source, const std::string& fragment_source) {
    GLuint vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_source);
    GLuint fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_source);
    
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetProgramInfoLog(program, 512, nullptr, info_log);
        throw std::runtime_error("Program linking failed: " + std::string(info_log));
    }
    
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    return program;
}

GLuint OpenGLContext::create_buffer(GLenum target, const void* data, size_t size, GLenum usage) {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    glBufferData(target, size, data, usage);
    return buffer;
}

GLuint OpenGLContext::create_vertex_array() {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    return vao;
}

void OpenGLContext::use_program(GLuint program) {
    current_program = program;
    glUseProgram(program);
}

void OpenGLContext::bind_vertex_array(GLuint vao) {
    glBindVertexArray(vao);
}

void OpenGLContext::bind_buffer(GLenum target, GLuint buffer) {
    glBindBuffer(target, buffer);
}

void OpenGLContext::set_uniform(GLuint program, const std::string& name, float value) {
    GLint loc = glGetUniformLocation(program, name.c_str());
    if (loc != -1) {
        glUniform1f(loc, value);
    }
}

void OpenGLContext::set_uniform(GLuint program, const std::string& name, const glm::vec3& value) {
    GLint loc = glGetUniformLocation(program, name.c_str());
    if (loc != -1) {
        glUniform3f(loc, value.x, value.y, value.z);
    }
}

void OpenGLContext::set_uniform(GLuint program, const std::string& name, const glm::mat4& value) {
    GLint loc = glGetUniformLocation(program, name.c_str());
    if (loc != -1) {
        glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
    }
}

OpenGLWindow::OpenGLWindow(int width, int height, const std::string& title)
    : width(width), height(height), title(title) {
}

OpenGLWindow::~OpenGLWindow() {
    if (window) {
        glfwDestroyWindow(window);
    }
}

void OpenGLWindow::init() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    
    make_context_current();
    
    // Set user pointer to this window
    glfwSetWindowUserPointer(window, this);
    
    // Set callbacks
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
}

void OpenGLWindow::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    OpenGLWindow* glfw_window = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(window));
    if (glfw_window && glfw_window->get_scene()) {
        // Get cursor position
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        if (action == GLFW_PRESS) {
            glfw_window->get_scene()->on_mouse_press(xpos, ypos, button, mods);
        } else if (action == GLFW_RELEASE) {
            glfw_window->get_scene()->on_mouse_release(xpos, ypos, button, mods);
        }
    }
}

void OpenGLWindow::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    OpenGLWindow* glfw_window = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(window));
    if (glfw_window && glfw_window->get_scene()) {
        glfw_window->get_scene()->on_mouse_motion(xpos, ypos);
    }
}

void OpenGLWindow::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    OpenGLWindow* glfw_window = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(window));
    if (glfw_window && glfw_window->get_scene()) {
        if (action == GLFW_PRESS) {
            glfw_window->get_scene()->on_key_press(key, mods);
        } else if (action == GLFW_RELEASE) {
            glfw_window->get_scene()->on_key_release(key, mods);
        }
    }
}

void OpenGLWindow::window_size_callback(GLFWwindow* window, int width, int height) {
    OpenGLWindow* glfw_window = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(window));
    if (glfw_window) {
        glfw_window->resize(width, height);
        if (glfw_window->get_scene()) {
            glfw_window->get_scene()->on_resize(width, height);
        }
    }
}

void OpenGLWindow::make_context_current() {
    glfwMakeContextCurrent(window);
}

void OpenGLWindow::swap_buffers() {
    glfwSwapBuffers(window);
}

void OpenGLWindow::poll_events() {
    glfwPollEvents();
}

bool OpenGLWindow::should_close() const {
    return glfwWindowShouldClose(window);
}

void OpenGLWindow::set_should_close(bool value) {
    glfwSetWindowShouldClose(window, value);
}

void OpenGLWindow::resize(int width, int height) {
    this->width = width;
    this->height = height;
    glViewport(0, 0, width, height);
}

ShaderWrapper::ShaderWrapper(const std::string& vertex_path, const std::string& fragment_path)
    : vertex_path(vertex_path), fragment_path(fragment_path) {
}

ShaderWrapper::~ShaderWrapper() {
    if (program != 0) {
        glDeleteProgram(program);
    }
}

void ShaderWrapper::init() {
    std::ifstream vertex_file(vertex_path);
    std::ifstream fragment_file(fragment_path);
    
    std::stringstream vertex_stream, fragment_stream;
    vertex_stream << vertex_file.rdbuf();
    fragment_stream << fragment_file.rdbuf();
    
    OpenGLContext ctx;
    program = ctx.create_program(vertex_stream.str(), fragment_stream.str());
}

void ShaderWrapper::bind() {
    glUseProgram(program);
}

void ShaderWrapper::unbind() {
    glUseProgram(0);
}

void ShaderWrapper::set_uniform(const std::string& name, float value) {
    GLint loc = glGetUniformLocation(program, name.c_str());
    if (loc != -1) {
        glUniform1f(loc, value);
    }
}

void ShaderWrapper::set_uniform(const std::string& name, const glm::vec3& value) {
    GLint loc = glGetUniformLocation(program, name.c_str());
    if (loc != -1) {
        glUniform3f(loc, value.x, value.y, value.z);
    }
}

void ShaderWrapper::set_uniform(const std::string& name, const glm::mat4& value) {
    GLint loc = glGetUniformLocation(program, name.c_str());
    if (loc != -1) {
        glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
    }
}

} // namespace manim