#pragma once

#include <thsan/gameObject/component/PhysicComponent.h>


class FixedOnGroundPhysicComponent : public PhysicComponent
{
public:
    FixedOnGroundPhysicComponent();

    void init(Scene& scene) override;
    void update(Scene& scene, const sf::Time& dt) override;
    ~FixedOnGroundPhysicComponent();
private:
    HitBox* hitbox;
};
