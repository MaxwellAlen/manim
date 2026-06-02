#pragma once

#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <unordered_map>

#include "manimcpp/constants.h"
#include "manimcpp/utils.h"

namespace manim {

class Mobject;
class Scene;

struct VertexData {
    glm::vec3 point;
    glm::vec4 rgba;
    
    VertexData() : point(0.0f), rgba(1.0f) {}
    VertexData(const glm::vec3& p, const glm::vec4& c) : point(p), rgba(c) {}
};

class Mobject {
public:
    int dim = 3;
    std::string shader_folder;
    int render_primitive = 4;
    bool depth_test = false;
    int z_index = 0;
    
    Mobject(const Color& color = DEFAULT_MOBJECT_COLOR, float opacity = 1.0f,
            const glm::vec3& shading = glm::vec3(0.0f), bool is_fixed_in_frame = false,
            bool depth_test = false, int z_index = 0);
    
    virtual ~Mobject();
    
    virtual std::string get_name() const { return typeid(*this).name(); }
    
    Mobject& operator+(Mobject& other);
    Mobject& operator*(int n);
    
    virtual void init_data(size_t length = 0);
    virtual void init_uniforms();
    virtual void init_colors();
    virtual void init_points();
    void init_updaters();
    
    void set_uniforms(const std::unordered_map<std::string, float>& uniforms);
    void set_uniform(const std::string& key, float value);
    void set_uniform(const std::string& key, const glm::vec3& value);
    
    void note_changed_data(bool recurse_up = true);
    
    void set_data(const std::vector<VertexData>& data);
    void resize_points(size_t new_length);
    void set_points(const std::vector<glm::vec3>& points);
    void append_points(const std::vector<glm::vec3>& new_points);
    
    std::vector<glm::vec3> get_points() const;
    size_t get_num_points() const;
    bool has_points() const;
    
    glm::mat3 get_bounding_box() const;
    void refresh_bounding_box(bool recurse_down = false, bool recurse_up = true);
    
    void add(Mobject* mobject);
    void remove(Mobject* mobject, bool reassemble = true, bool recurse = true);
    void clear();
    void add_to_back(Mobject* mobject);
    
    std::vector<Mobject*> get_family(bool recurse = true) const;
    
    void shift(const glm::vec3& vector);
    void scale(float scale_factor, const glm::vec3& about_point = glm::vec3(0.0f));
    void rotate(float angle, const glm::vec3& axis = OUT, const glm::vec3& about_point = glm::vec3(0.0f));
    void flip(const glm::vec3& axis = UP);
    
    void center();
    void move_to(const glm::vec3& point);
    void move_to(const Mobject& mobject);
    void next_to(const Mobject& mobject, const glm::vec3& direction = RIGHT, float buff = DEFAULT_MOBJECT_TO_MOBJECT_BUFF);
    
    void set_color(const Color& color, float opacity = 1.0f);
    void set_opacity(float opacity);
    Color get_color() const;
    float get_opacity() const;
    
    glm::vec3 get_center() const;
    float get_width() const;
    float get_height() const;
    float get_depth() const;
    
    void set_width(float width);
    void set_height(float height);
    void set_depth(float depth);
    
    virtual void update(float dt);
    void add_updater(std::function<void(Mobject*, float)> updater);
    void remove_updater(size_t index);
    bool has_updaters() const;
    
    bool is_updating_suspended() const { return updating_suspended; }
    void suspend_updating() { updating_suspended = true; }
    void resume_updating() { updating_suspended = false; }
    
    virtual void render(class OpenGLContext& ctx, const std::unordered_map<std::string, float>& uniforms);
    
    virtual Mobject* copy() const;
    
protected:
    std::vector<VertexData> data;
    VertexData data_default;
    
    std::vector<Mobject*> submobjects;
    std::vector<Mobject*> parents;
    mutable std::vector<Mobject*> family_cache;
    mutable bool family_dirty = true;
    
    std::unordered_map<std::string, float> float_uniforms;
    std::unordered_map<std::string, glm::vec3> vec3_uniforms;
    
    std::vector<std::function<void(Mobject*, float)>> updaters;
    bool updating_suspended = false;
    
    mutable glm::mat3 bounding_box;
    mutable bool needs_new_bounding_box = true;
    bool data_has_changed = true;
    
    Color color;
    float opacity;
    glm::vec3 shading;
    
    bool is_fixed_in_frame = false;
    
    void compute_bounding_box() const;
    glm::vec3 get_bounding_box_point(const glm::vec3& direction) const;
};

class Group : public Mobject {
public:
    Group();
    Group(const std::initializer_list<Mobject*>& mobjects);
    
    void add(Mobject* mobject);
    void remove(Mobject* mobject);
    
    Mobject* operator[](size_t index);
    const Mobject* operator[](size_t index) const;
    size_t size() const;
};

} // namespace manim