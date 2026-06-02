#pragma once

#include <vector>
#include <memory>
#include <string>
#include <functional>

#include "manimcpp/constants.h"
#include "manimcpp/mobject.h"

namespace manim {

class Scene;

class Animation {
public:
    static constexpr float DEFAULT_ANIMATION_RUN_TIME = 1.0f;
    static constexpr float DEFAULT_ANIMATION_LAG_RATIO = 0.0f;
    
    Animation(Mobject* mobject, float run_time = DEFAULT_ANIMATION_RUN_TIME,
              float lag_ratio = DEFAULT_ANIMATION_LAG_RATIO,
              std::function<float(float)> rate_func = smooth,
              const std::string& name = "", bool remover = false,
              float final_alpha_value = 1.0f,
              bool suspend_mobject_updating = false);
    
    virtual ~Animation();
    
    virtual void begin();
    virtual void finish();
    virtual void clean_up_from_scene(Scene* scene);
    
    virtual void update_mobjects(float dt);
    virtual void interpolate(float alpha);
    
    void update_rate_info(float run_time = -1.0f,
                          std::function<float(float)> rate_func = nullptr,
                          float lag_ratio = -1.0f);
    
    float get_run_time() const;
    void set_run_time(float run_time);
    
    std::function<float(float)> get_rate_func() const;
    void set_rate_func(std::function<float(float)> rate_func);
    
    bool is_remover() const { return remover; }
    
    Mobject* get_mobject() const { return mobject; }
    
protected:
    Mobject* mobject = nullptr;
    std::unique_ptr<Mobject> starting_mobject;
    
    float run_time;
    float lag_ratio;
    std::function<float(float)> rate_func;
    std::string name;
    bool remover;
    float final_alpha_value;
    bool suspend_mobject_updating;
    bool mobject_was_updating = false;
    
    std::vector<std::pair<Mobject*, Mobject*>> families;
    
    virtual void interpolate_mobject(float alpha);
    virtual void interpolate_submobject(Mobject* submobject, Mobject* starting_submobject, float alpha);
    
    float get_sub_alpha(float alpha, int index, int num_submobjects);
};

class Transform : public Animation {
public:
    Transform(Mobject* mobject, Mobject* target_mobject,
              float run_time = DEFAULT_ANIMATION_RUN_TIME,
              float lag_ratio = DEFAULT_ANIMATION_LAG_RATIO,
              std::function<float(float)> rate_func = smooth);
    
    void begin() override;
    void update_mobjects(float dt) override;
    void interpolate_submobject(Mobject* submobject, Mobject* starting_submobject, float alpha) override;
    
protected:
    std::unique_ptr<Mobject> target_mobject;
};

class FadeIn : public Animation {
public:
    FadeIn(Mobject* mobject, float run_time = DEFAULT_ANIMATION_RUN_TIME,
           float lag_ratio = DEFAULT_ANIMATION_LAG_RATIO);
    
    void begin() override;
    void interpolate_mobject(float alpha) override;
};

class FadeOut : public Animation {
public:
    FadeOut(Mobject* mobject, float run_time = DEFAULT_ANIMATION_RUN_TIME,
            float lag_ratio = DEFAULT_ANIMATION_LAG_RATIO);
    
    void interpolate_mobject(float alpha) override;
};

class GrowFromCenter : public Animation {
public:
    GrowFromCenter(Mobject* mobject, float run_time = DEFAULT_ANIMATION_RUN_TIME);
    
    void begin() override;
    void interpolate_mobject(float alpha) override;
    
protected:
    glm::vec3 original_center;
};

class Rotate : public Animation {
public:
    Rotate(Mobject* mobject, float angle, const glm::vec3& axis = OUT,
           float run_time = DEFAULT_ANIMATION_RUN_TIME,
           float lag_ratio = DEFAULT_ANIMATION_LAG_RATIO);
    
    void begin() override;
    void interpolate_mobject(float alpha) override;
    
protected:
    float angle;
    glm::vec3 axis;
    float original_angle = 0.0f;
};

class ScaleInPlace : public Animation {
public:
    ScaleInPlace(Mobject* mobject, float scale_factor,
                 float run_time = DEFAULT_ANIMATION_RUN_TIME);
    
    void begin() override;
    void interpolate_mobject(float alpha) override;
    
protected:
    float scale_factor;
    float original_scale = 1.0f;
};

class AnimationGroup : public Animation {
public:
    AnimationGroup(const std::vector<Animation*>& animations,
                   float run_time = DEFAULT_ANIMATION_RUN_TIME,
                   bool lagged = false);
    
    void begin() override;
    void finish() override;
    void update_mobjects(float dt) override;
    void interpolate(float alpha) override;
    
protected:
    std::vector<Animation*> animations;
};

} // namespace manim