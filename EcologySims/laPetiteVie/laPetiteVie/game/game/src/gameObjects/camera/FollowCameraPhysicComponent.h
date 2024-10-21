#pragma once

#include <thsan/gameObject/component/PhysicComponent.h>


class FollowCameraPhysicComponent : public PhysicComponent
{
    public:
        FollowCameraPhysicComponent(GameObject* player);

        void init(Scene& scene) override;
        void update(Scene& scene, const sf::Time& dt) override;

        ~FollowCameraPhysicComponent();
	private:
        void setBeginAngle(CameraData* cd);
        void setEndAngle(glm::vec3 target_position, glm::vec3 player_position, float range_angle_degree, CameraData* cd);
        
        void setBeginHorizon(float horizon);
        void setEndHorizon(float horizon, VoxelTerrainData* vt);

        void interpolateCamera(const sf::Time& dt);
        void interpolateCameraAngle(const sf::Time& dt);
        void interpolateCameraHorizon(const sf::Time& dt);

		GameObject* target;
		Scene* p_scene;
        const std::vector<std::string>* renderer_tags;
        PlayerData* player_data;
        sf::FloatRect target_rect;

        const sf::Time CAMERA_ANGLE_INTERPOLATION_TIME = sf::seconds(0.5f);
        const sf::Time CAMERA_HORIZON_INTERPOLATION_TIME = sf::seconds(0.5f);

        sf::Time t_angle = sf::seconds(0.f);
        sf::Time t_horizon = sf::seconds(0.f);

        float begin_angle{ 0.f };
        float end_angle{ 0.f };

        float begin_horizon{ 0.f };
        float end_horizon{ 0.f };

        float* curr_angle{ nullptr };
        float* curr_horizon{ nullptr };

        float curr_offset{ 200.f };
        float speed = 20.f;

        float base_horizon{ std::numeric_limits<float>::max() };

        bool is_interpolating{ false };
        bool is_following{ false };

};

