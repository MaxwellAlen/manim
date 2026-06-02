#include "manimcpp/mobject.h"

namespace manim {

Mobject::Mobject(const Color& color, float opacity, const glm::vec3& shading,
                 bool is_fixed_in_frame, bool depth_test, int z_index)
    : color(color), opacity(opacity), shading(shading),
      is_fixed_in_frame(is_fixed_in_frame), depth_test(depth_test), z_index(z_index) {
    init_data();
    init_uniforms();
    init_updaters();
    init_colors();
    init_points();
}

Mobject::~Mobject() {
    for (auto sm : submobjects) {
        delete sm;
    }
}

void Mobject::init_data(size_t length) {
    data.resize(length);
    data_default = VertexData();
    if (length > 0) {
        data_default = data[0];
    }
}

void Mobject::init_uniforms() {
    float_uniforms["is_fixed_in_frame"] = is_fixed_in_frame ? 1.0f : 0.0f;
    vec3_uniforms["shading"] = shading;
    vec3_uniforms["clip_plane0"] = glm::vec3(0.0f);
    vec3_uniforms["clip_plane1"] = glm::vec3(0.0f);
    vec3_uniforms["clip_plane2"] = glm::vec3(0.0f);
    vec3_uniforms["clip_plane3"] = glm::vec3(0.0f);
}

void Mobject::init_colors() {
    set_color(color, opacity);
}

void Mobject::init_points() {
}

void Mobject::init_updaters() {
    updaters.clear();
    updating_suspended = false;
}

void Mobject::set_uniforms(const std::unordered_map<std::string, float>& uniforms) {
    for (const auto& [key, value] : uniforms) {
        float_uniforms[key] = value;
    }
}

void Mobject::set_uniform(const std::string& key, float value) {
    float_uniforms[key] = value;
}

void Mobject::set_uniform(const std::string& key, const glm::vec3& value) {
    vec3_uniforms[key] = value;
}

void Mobject::note_changed_data(bool recurse_up) {
    data_has_changed = true;
    needs_new_bounding_box = true;
    if (recurse_up) {
        for (auto parent : parents) {
            parent->note_changed_data();
        }
    }
}

void Mobject::set_data(const std::vector<VertexData>& new_data) {
    data = new_data;
    note_changed_data();
}

void Mobject::resize_points(size_t new_length) {
    data.resize(new_length, data_default);
    note_changed_data();
}

void Mobject::set_points(const std::vector<glm::vec3>& points) {
    init_data(points.size());
    for (size_t i = 0; i < points.size(); ++i) {
        data[i].point = points[i];
        data[i].rgba = glm::vec4(color.r, color.g, color.b, opacity);
    }
    note_changed_data();
}

void Mobject::append_points(const std::vector<glm::vec3>& new_points) {
    size_t n = data.size();
    resize_points(n + new_points.size());
    for (size_t i = 0; i < new_points.size(); ++i) {
        data[n + i].point = new_points[i];
    }
    note_changed_data();
}

std::vector<glm::vec3> Mobject::get_points() const {
    std::vector<glm::vec3> result;
    result.reserve(data.size());
    for (const auto& vd : data) {
        result.push_back(vd.point);
    }
    return result;
}

size_t Mobject::get_num_points() const {
    return data.size();
}

bool Mobject::has_points() const {
    return !data.empty();
}

glm::mat3 Mobject::get_bounding_box() const {
    if (needs_new_bounding_box) {
        compute_bounding_box();
    }
    return bounding_box;
}

void Mobject::compute_bounding_box() const {
    if (data.empty()) {
        bounding_box = glm::mat3(0.0f);
        needs_new_bounding_box = false;
        return;
    }
    
    glm::vec3 mins = data[0].point;
    glm::vec3 maxs = data[0].point;
    
    for (const auto& vd : data) {
        mins = glm::min(mins, vd.point);
        maxs = glm::max(maxs, vd.point);
    }
    
    glm::vec3 mids = (mins + maxs) / 2.0f;
    
    bounding_box[0] = mins;
    bounding_box[1] = mids;
    bounding_box[2] = maxs;
    
    needs_new_bounding_box = false;
}

void Mobject::refresh_bounding_box(bool recurse_down, bool recurse_up) {
    if (recurse_down) {
        for (auto sm : submobjects) {
            sm->refresh_bounding_box(true, false);
        }
    }
    needs_new_bounding_box = true;
    if (recurse_up) {
        for (auto parent : parents) {
            parent->refresh_bounding_box(false, true);
        }
    }
}

glm::vec3 Mobject::get_bounding_box_point(const glm::vec3& direction) const {
    glm::mat3 bb = get_bounding_box();
    glm::vec3 result = bb[1];
    for (int i = 0; i < 3; ++i) {
        if (direction[i] > 0) {
            result[i] = bb[2][i];
        } else if (direction[i] < 0) {
            result[i] = bb[0][i];
        }
    }
    return result;
}

void Mobject::add(Mobject* mobject) {
    if (mobject == this) return;
    if (std::find(submobjects.begin(), submobjects.end(), mobject) == submobjects.end()) {
        submobjects.push_back(mobject);
    }
    if (std::find(mobject->parents.begin(), mobject->parents.end(), this) == mobject->parents.end()) {
        mobject->parents.push_back(this);
    }
    family_dirty = true;
    refresh_bounding_box();
}

void Mobject::remove(Mobject* mobject, bool reassemble, bool recurse) {
    auto it = std::find(submobjects.begin(), submobjects.end(), mobject);
    if (it != submobjects.end()) {
        submobjects.erase(it);
    }
    auto pit = std::find(mobject->parents.begin(), mobject->parents.end(), this);
    if (pit != mobject->parents.end()) {
        mobject->parents.erase(pit);
    }
    family_dirty = true;
    if (reassemble) {
        refresh_bounding_box();
    }
}

void Mobject::clear() {
    for (auto sm : submobjects) {
        remove(sm, false, false);
    }
    submobjects.clear();
}

void Mobject::add_to_back(Mobject* mobject) {
    auto it = std::find(submobjects.begin(), submobjects.end(), mobject);
    if (it != submobjects.end()) {
        submobjects.erase(it);
    }
    submobjects.insert(submobjects.begin(), mobject);
    family_dirty = true;
}

std::vector<Mobject*> Mobject::get_family(bool recurse) const {
    if (!recurse) {
        return {const_cast<Mobject*>(this)};
    }
    if (!family_dirty && !family_cache.empty()) {
        return family_cache;
    }
    
    family_cache.clear();
    family_cache.push_back(const_cast<Mobject*>(this));
    for (auto sm : submobjects) {
        auto sm_family = sm->get_family(true);
        family_cache.insert(family_cache.end(), sm_family.begin(), sm_family.end());
    }
    family_dirty = false;
    return family_cache;
}

void Mobject::shift(const glm::vec3& vector) {
    for (auto& vd : data) {
        vd.point += vector;
    }
    for (auto sm : submobjects) {
        sm->shift(vector);
    }
    note_changed_data();
}

void Mobject::scale(float scale_factor, const glm::vec3& about_point) {
    glm::vec3 center = about_point;
    if (center == glm::vec3(0.0f)) {
        center = get_center();
    }
    
    for (auto& vd : data) {
        vd.point = center + scale_factor * (vd.point - center);
    }
    for (auto sm : submobjects) {
        sm->scale(scale_factor, center);
    }
    note_changed_data();
}

void Mobject::rotate(float angle, const glm::vec3& axis, const glm::vec3& about_point) {
    glm::vec3 center = about_point;
    if (center == glm::vec3(0.0f)) {
        center = get_center();
    }
    
    glm::mat4 rot = rotation_matrix(angle, axis);
    for (auto& vd : data) {
        glm::vec4 p = glm::vec4(vd.point - center, 1.0f);
        vd.point = glm::vec3(rot * p) + center;
    }
    for (auto sm : submobjects) {
        sm->rotate(angle, axis, center);
    }
    note_changed_data();
}

void Mobject::flip(const glm::vec3& axis) {
    rotate(PI, axis);
}

void Mobject::center() {
    shift(-get_center());
}

void Mobject::move_to(const glm::vec3& point) {
    shift(point - get_center());
}

void Mobject::move_to(const Mobject& mobject) {
    move_to(mobject.get_center());
}

void Mobject::next_to(const Mobject& mobject, const glm::vec3& direction, float buff) {
    glm::vec3 target_point = mobject.get_bounding_box_point(direction);
    glm::vec3 my_point = get_bounding_box_point(-direction);
    shift(target_point - my_point + buff * direction);
}

void Mobject::set_color(const Color& new_color, float new_opacity) {
    color = new_color;
    opacity = new_opacity;
    for (auto& vd : data) {
        vd.rgba = glm::vec4(color.r, color.g, color.b, opacity);
    }
    for (auto sm : submobjects) {
        sm->set_color(new_color, new_opacity);
    }
    note_changed_data();
}

void Mobject::set_opacity(float new_opacity) {
    opacity = new_opacity;
    for (auto& vd : data) {
        vd.rgba.a = opacity;
    }
    for (auto sm : submobjects) {
        sm->set_opacity(new_opacity);
    }
    note_changed_data();
}

Color Mobject::get_color() const {
    if (data.empty()) {
        return color;
    }
    const auto& rgba = data[0].rgba;
    return Color(rgba.r, rgba.g, rgba.b, rgba.a);
}

float Mobject::get_opacity() const {
    if (data.empty()) {
        return opacity;
    }
    return data[0].rgba.a;
}

glm::vec3 Mobject::get_center() const {
    glm::mat3 bb = get_bounding_box();
    return bb[1];
}

float Mobject::get_width() const {
    glm::mat3 bb = get_bounding_box();
    return bb[2][0] - bb[0][0];
}

float Mobject::get_height() const {
    glm::mat3 bb = get_bounding_box();
    return bb[2][1] - bb[0][1];
}

float Mobject::get_depth() const {
    glm::mat3 bb = get_bounding_box();
    return bb[2][2] - bb[0][2];
}

void Mobject::set_width(float width) {
    float current_width = get_width();
    if (current_width > 0.0001f) {
        scale(width / current_width);
    }
}

void Mobject::set_height(float height) {
    float current_height = get_height();
    if (current_height > 0.0001f) {
        scale(height / current_height);
    }
}

void Mobject::set_depth(float depth) {
    float current_depth = get_depth();
    if (current_depth > 0.0001f) {
        scale(depth / current_depth);
    }
}

void Mobject::update(float dt) {
    if (updating_suspended) return;
    for (auto sm : submobjects) {
        sm->update(dt);
    }
    for (auto& updater : updaters) {
        updater(this, dt);
    }
}

void Mobject::add_updater(std::function<void(Mobject*, float)> updater) {
    updaters.push_back(updater);
}

void Mobject::remove_updater(size_t index) {
    if (index < updaters.size()) {
        updaters.erase(updaters.begin() + index);
    }
}

bool Mobject::has_updaters() const {
    return !updaters.empty();
}

void Mobject::render(class OpenGLContext& ctx, const std::unordered_map<std::string, float>& uniforms) {
}

Mobject* Mobject::copy() const {
    Mobject* result = new Mobject(color, opacity, shading, is_fixed_in_frame, depth_test, z_index);
    result->data = data;
    result->submobjects.reserve(submobjects.size());
    for (auto sm : submobjects) {
        result->add(sm->copy());
    }
    return result;
}

Mobject& Mobject::operator+(Mobject& other) {
    Group* group = new Group();
    group->add(this);
    group->add(&other);
    return *group;
}

Mobject& Mobject::operator*(int n) {
    Group* group = new Group();
    for (int i = 0; i < n; ++i) {
        group->add(copy());
    }
    return *group;
}

Group::Group() : Mobject() {}

Group::Group(const std::initializer_list<Mobject*>& mobjects) : Mobject() {
    for (auto m : mobjects) {
        add(m);
    }
}

void Group::add(Mobject* mobject) {
    Mobject::add(mobject);
}

void Group::remove(Mobject* mobject) {
    Mobject::remove(mobject);
}

Mobject* Group::operator[](size_t index) {
    return submobjects[index];
}

const Mobject* Group::operator[](size_t index) const {
    return submobjects[index];
}

size_t Group::size() const {
    return submobjects.size();
}

} // namespace manim