#pragma once

#include "../component/PhysicComponent.h"
#include "../../graphic/PostProcess.h"

class VolumetricPostProcessingPhysicComponent : public PhysicComponent
{
public:
    VolumetricPostProcessingPhysicComponent(GameObject* target);
    ~VolumetricPostProcessingPhysicComponent() = default;

    void init(Scene& scene) override;
    void update(Scene& scene, const sf::Time& dt) override;
private:
    static bool locked;

    void interpolate_effect(const sf::Time& dt);
    void init_interpolate(PostProcess::Effect effect, float final_opacity, Ease e, InterpolationType it, sf::Time duration);
    void enable_then_init_interpolate(PostProcess::Effect effect, float final_opacity, Ease e, InterpolationType it, sf::Time duration);
    void init_interpolate_then_disable(PostProcess::Effect effect, Ease e, InterpolationType it, sf::Time duration);
    
    std::shared_ptr<PostProcess> pp;
    HitBox* hitbox;
    VolumetricPostProcessData* vol_pp_data;

    std::unordered_map<PostProcess::Effect, float> begin_opacity;
    std::unordered_map<PostProcess::Effect, float> curr_opacity;
    std::unordered_map<PostProcess::Effect, float> default_opacity;
    std::unordered_map<PostProcess::Effect, float> final_opacity;

    std::unordered_map<PostProcess::Effect, bool> started_enable;

    sf::Time elapsed_time{ sf::seconds(0.f) };
    sf::Time duration{ sf::seconds(0.f) };
    
    bool isInterpolating{ false };
    bool isMarkedForDisabling{ false };
    bool isFirstInterlapsing{ true };
    bool isInterpolationFullyDone{ false };

    Ease ease;
    InterpolationType interpolate_type;

    Transform* transform;
    GameObject* target;
    bool isOverlapping{false};
};