#pragma once

#include <thsan/gameObject/component/PhysicComponent.h>


class PlayerPhysicComponent : public PhysicComponent
{
    public:
        PlayerPhysicComponent();

        void init(Scene& scene) override;
        void update(Scene& scene, const sf::Time& dt) override;
        ~PlayerPhysicComponent() = default;
    private:
        bool onGround{ true };
        bool isJumping{ false };
        bool isDoubleJumping{ false };
        bool isAiming{ false };
        bool isReachingTarget{ false };
        float y0{0.f};
        float y01{0.f};
        float x0{0.f};
        float x01{0.f};
        float z0{0.f};
        float z01{0.f};
        glm::vec3 velocity{0.f};
        float vx;
        float th{0.5f};
        float h{1.5f};
        float max_walking_speed{ 2.f };
        float max_running_speed{ 5.f };
        glm::vec3 wall_normal;
        GameObject* target_aim;
        sf::Time jump_timer;
        sf::Time double_jump_timer;
        sf::Time aim_timer;
};
