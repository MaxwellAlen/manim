#include "manimcpp/animation.h"
#include "manimcpp/scene.h"

namespace manim {

Animation::Animation(Mobject* mobject, float run_time, float lag_ratio,
                     std::function<float(float)> rate_func, const std::string& name,
                     bool remover, float final_alpha_value,
                     bool suspend_mobject_updating)
    : mobject(mobject), run_time(run_time), lag_ratio(lag_ratio),
      rate_func(rate_func), name(name), remover(remover),
      final_alpha_value(final_alpha_value),
      suspend_mobject_updating(suspend_mobject_updating) {
}

Animation::~Animation() {
}

void Animation::begin() {
    starting_mobject = std::unique_ptr<Mobject>(mobject->copy());
    if (suspend_mobject_updating) {
        mobject_was_updating = !mobject->is_updating_suspended();
        mobject->suspend_updating();
    }
    
    auto family1 = mobject->get_family();
    auto family2 = starting_mobject->get_family();
    
    for (size_t i = 0; i < std::min(family1.size(), family2.size()); ++i) {
        families.emplace_back(family1[i], family2[i]);
    }
    
    interpolate(0.0f);
}

void Animation::finish() {
    interpolate(final_alpha_value);
    if (suspend_mobject_updating && mobject_was_updating) {
        mobject->resume_updating();
    }
}

void Animation::clean_up_from_scene(Scene* scene) {
    if (remover) {
        scene->remove(mobject);
    }
}

void Animation::update_mobjects(float dt) {
    starting_mobject->update(dt);
}

void Animation::interpolate(float alpha) {
    interpolate_mobject(rate_func(alpha));
}

void Animation::update_rate_info(float run_time,
                                 std::function<float(float)> rate_func,
                                 float lag_ratio) {
    if (run_time >= 0.0f) {
        this->run_time = run_time;
    }
    if (rate_func) {
        this->rate_func = rate_func;
    }
    if (lag_ratio >= 0.0f) {
        this->lag_ratio = lag_ratio;
    }
}

float Animation::get_run_time() const {
    return run_time;
}

void Animation::set_run_time(float run_time) {
    this->run_time = run_time;
}

std::function<float(float)> Animation::get_rate_func() const {
    return rate_func;
}

void Animation::set_rate_func(std::function<float(float)> rate_func) {
    this->rate_func = rate_func;
}

void Animation::interpolate_mobject(float alpha) {
    for (size_t i = 0; i < families.size(); ++i) {
        float sub_alpha = get_sub_alpha(alpha, i, families.size());
        interpolate_submobject(families[i].first, families[i].second, sub_alpha);
    }
}

void Animation::interpolate_submobject(Mobject* submobject, Mobject* starting_submobject, float alpha) {
}

float Animation::get_sub_alpha(float alpha, int index, int num_submobjects) {
    if (num_submobjects <= 1) {
        return alpha;
    }
    
    float lag_ratio = this->lag_ratio;
    float full_length = (num_submobjects - 1) * lag_ratio + 1;
    float value = alpha * full_length;
    float lower = index * lag_ratio;
    float raw_sub_alpha = std::max(0.0f, std::min(1.0f, value - lower));
    return rate_func(raw_sub_alpha);
}

Transform::Transform(Mobject* mobject, Mobject* target_mobject,
                     float run_time, float lag_ratio,
                     std::function<float(float)> rate_func)
    : Animation(mobject, run_time, lag_ratio, rate_func) {
    this->target_mobject = std::unique_ptr<Mobject>(target_mobject->copy());
}

void Transform::begin() {
    Animation::begin();
}

void Transform::update_mobjects(float dt) {
    Animation::update_mobjects(dt);
    target_mobject->update(dt);
}

void Transform::interpolate_submobject(Mobject* submobject, Mobject* starting_submobject, float alpha) {
    auto start_points = starting_submobject->get_points();
    auto target_points = target_mobject->get_points();
    
    size_t n = std::min(start_points.size(), target_points.size());
    std::vector<glm::vec3> new_points;
    new_points.reserve(n);
    
    for (size_t i = 0; i < n; ++i) {
        new_points.push_back(manim::interpolate(start_points[i], target_points[i], alpha));
    }
    
    submobject->set_points(new_points);
}

FadeIn::FadeIn(Mobject* mobject, float run_time, float lag_ratio)
    : Animation(mobject, run_time, lag_ratio) {
}

void FadeIn::begin() {
    Animation::begin();
    mobject->set_opacity(0.0f);
}

void FadeIn::interpolate_mobject(float alpha) {
    mobject->set_opacity(alpha);
}

FadeOut::FadeOut(Mobject* mobject, float run_time, float lag_ratio)
    : Animation(mobject, run_time, lag_ratio) {
    remover = true;
}

void FadeOut::interpolate_mobject(float alpha) {
    mobject->set_opacity(1.0f - alpha);
}

GrowFromCenter::GrowFromCenter(Mobject* mobject, float run_time)
    : Animation(mobject, run_time) {
}

void GrowFromCenter::begin() {
    Animation::begin();
    original_center = mobject->get_center();
    mobject->scale(0.0f);
    mobject->move_to(original_center);
}

void GrowFromCenter::interpolate_mobject(float alpha) {
    mobject->scale(0.0f);
    mobject->move_to(original_center);
    mobject->scale(alpha);
}

Rotate::Rotate(Mobject* mobject, float angle, const glm::vec3& axis,
               float run_time, float lag_ratio)
    : Animation(mobject, run_time, lag_ratio), angle(angle), axis(axis) {
}

void Rotate::begin() {
    Animation::begin();
    original_angle = 0.0f;
}

void Rotate::interpolate_mobject(float alpha) {
    float current_angle = alpha * angle;
    mobject->rotate(current_angle - original_angle, axis);
    original_angle = current_angle;
}

ScaleInPlace::ScaleInPlace(Mobject* mobject, float scale_factor, float run_time)
    : Animation(mobject, run_time), scale_factor(scale_factor) {
}

void ScaleInPlace::begin() {
    Animation::begin();
    original_scale = 1.0f;
}

void ScaleInPlace::interpolate_mobject(float alpha) {
    float target_scale = 1.0f + (scale_factor - 1.0f) * alpha;
    mobject->scale(target_scale / original_scale);
    original_scale = target_scale;
}

AnimationGroup::AnimationGroup(const std::vector<Animation*>& animations,
                               float run_time, bool lagged)
    : Animation(nullptr, run_time), animations(animations) {
}

void AnimationGroup::begin() {
    for (auto anim : animations) {
        anim->begin();
    }
}

void AnimationGroup::finish() {
    for (auto anim : animations) {
        anim->finish();
    }
}

void AnimationGroup::update_mobjects(float dt) {
    for (auto anim : animations) {
        anim->update_mobjects(dt);
    }
}

void AnimationGroup::interpolate(float alpha) {
    for (auto anim : animations) {
        anim->interpolate(alpha);
    }
}

} // namespace manim