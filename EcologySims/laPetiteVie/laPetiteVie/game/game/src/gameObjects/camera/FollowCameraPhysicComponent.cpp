#include "FollowCameraPhysicComponent.h"

#include <iostream>
#include <SFML/Graphics/Glsl.hpp>
#include <glm/gtx/easing.hpp>
#include <thsan/gameObject/componentData/ComponentData.h>

FollowCameraPhysicComponent::FollowCameraPhysicComponent(GameObject* player):
	target(player)
{
}

void FollowCameraPhysicComponent::init(Scene& scene)
{
	p_scene = &scene;

	if (!parent->hasComponent(COMPONENT_DATA::CAMERA))
		parent->setComponent(COMPONENT_DATA::CAMERA, new CameraData());
	CameraData* cd = parent->getComponent<CameraData>(COMPONENT_DATA::CAMERA);
	cd->target_name = target->getName();

	cd->offset.y = 100.f;
	cd->offset.z = 100.f;
	cd->angle = 0.f;
	cd->offset_distance = 100.f;
	cd->isPolar = true;

	curr_offset = cd->offset_distance;
}


void FollowCameraPhysicComponent::update(Scene& scene, const sf::Time& dt)
{
	CameraData* cd = parent->getComponent<CameraData>(COMPONENT_DATA::CAMERA);
	VoxelTerrainData* vt = scene.get_child("terrain")->getComponent<VoxelTerrainData>(COMPONENT_DATA::VOXEL_TERRAIN);
	Transform* player_transform = scene.get_child("player")->getComponent<Transform>(COMPONENT_DATA::TRANSFORM);
	Transform* transform = parent->getComponent<Transform>(COMPONENT_DATA::TRANSFORM);

	glm::vec3 position = player_transform->position;

	cd->offset_distance = 50.f + position.y / 8.f;

	if (vt) {

		if (base_horizon == std::numeric_limits<float>::max())
			base_horizon = vt->horizon;

		if (target) {
			if (cd->target_name != target->getName()) {
				target = scene.get_child(cd->target_name);
			}
			if (target && scene.pseudo3DRendering_getViewType() == VIEW_TYPE::DYNAMIC_VIEW) {

				vt->angle = cd->angle;
				curr_offset = cd->offset_distance;
				float x = curr_offset * sin(cd->angle);
				float z = curr_offset * cos(cd->angle);

				glm::vec3 target_position = target->getComponent<Transform>(COMPONENT_DATA::TRANSFORM)->position;
				glm::vec3 camera_position = target_position + glm::vec3(x, cd->offset.y, z);

				float height{0.f};
				/*
				do {
					
					height = scene.getPhysicalHeightOnPosition(sf::Vector2f(camera_position.x, camera_position.z));
					height -= camera_position.y;
					if (height > 0.f) {
						if (height < 20.f)
							camera_position.y += height / 2.f;

						curr_offset -= 1.f;
						x = curr_offset * sin(cd->angle);
						z = curr_offset * cos(cd->angle);
						camera_position = target_position + glm::vec3(x, cd->offset.y, z);
					}
				} while (height > 0.f);
				*/

				vt->position = sf::Vector3f(camera_position.x, camera_position.y, camera_position.z);
				transform->position = camera_position;

			}
		}
		else
			target = scene.get_child(cd->target_name);

		if(scene.pseudo3DRendering_getViewType() == VIEW_TYPE::DYNAMIC_VIEW)
			interpolateCamera(dt);

		/*
		*	aiming if target is a player and wants to aim his own target
		*/

		if(!renderer_tags)
			renderer_tags = scene.getAllTags();


		glm::vec3 player_position;
		float middle = scene.getRenderResolution().y / 2.f;

		if (target->hasComponent(COMPONENT_DATA::PLAYER)) {
			player_data = target->getComponent<PlayerData>(COMPONENT_DATA::PLAYER);
			player_position = target->getComponent<Transform>(COMPONENT_DATA::TRANSFORM)->position;
		}else
			player_data = nullptr;

		if (player_data) {

			if (player_data->target_name == "") {
				t_angle = sf::seconds(0.f);
				t_horizon = sf::seconds(0.f);
				curr_angle = nullptr;
				curr_horizon = nullptr;
			}
		

			for (const auto& tag : *renderer_tags) {

				if (tag == player_data->target_name) {
					is_following = true;
					glm::vec3 target_position = scene.get_child(player_data->target_name)->getComponent<Transform>(COMPONENT_DATA::TRANSFORM)->position;
					if (!curr_angle || t_angle >= CAMERA_ANGLE_INTERPOLATION_TIME) {
						setBeginAngle(cd);
					}

					if (!curr_horizon || t_horizon >= CAMERA_HORIZON_INTERPOLATION_TIME) {
						setBeginHorizon(vt->horizon);
					}

					setEndAngle(target_position, player_position, 50.f, cd);

					auto player_rect = scene.querySpriteRectAssociateWith(target->getName());
					auto target_rect = scene.querySpriteRectAssociateWith(player_data->target_name);
					float screen_height = (target_rect.top + target_rect.height / 2.f) + (player_rect.top + player_rect.height / 2.f);
					screen_height /= 2.f;
					setEndHorizon(middle + (middle - screen_height/2), vt);

				}

			}
		}


	}
}

FollowCameraPhysicComponent::~FollowCameraPhysicComponent()
{
    //dtor
}

void FollowCameraPhysicComponent::interpolateCamera(const sf::Time& dt)
{
	interpolateCameraAngle(dt);
	interpolateCameraHorizon(dt);
}

void FollowCameraPhysicComponent::interpolateCameraAngle(const sf::Time& dt)
{
	if (!curr_angle)
		return;

	t_angle += dt;

	if (t_angle > CAMERA_ANGLE_INTERPOLATION_TIME) {
		t_angle = CAMERA_ANGLE_INTERPOLATION_TIME;
		curr_angle = nullptr;
	}
	float s = glm::cubicEaseOut<float>(t_angle.asSeconds() / CAMERA_ANGLE_INTERPOLATION_TIME.asSeconds());
	if(curr_angle)
		*curr_angle = begin_angle * (1.f - s) + end_angle * s;

}

void FollowCameraPhysicComponent::interpolateCameraHorizon(const sf::Time& dt)
{
	if (!curr_horizon)
		return;

	t_horizon += dt;
	if (t_horizon > CAMERA_HORIZON_INTERPOLATION_TIME) {
		t_horizon = CAMERA_HORIZON_INTERPOLATION_TIME;
		curr_horizon = nullptr;
	}
	float s = glm::cubicEaseOut<float>(t_horizon.asSeconds() / CAMERA_HORIZON_INTERPOLATION_TIME.asSeconds());
	if(curr_horizon)
		*curr_horizon = begin_horizon * (1.f - s) + end_horizon * s;

}


void FollowCameraPhysicComponent::setBeginAngle(CameraData* cd)
{
	begin_angle = cd->angle;
}

void FollowCameraPhysicComponent::setEndAngle(glm::vec3 target_position, glm::vec3 player_position, float range_angle_degree, CameraData* cd)
{
	glm::vec2 n = glm::vec2(cos(begin_angle), sin(begin_angle));

	//X axis of device coordinate
	glm::vec2 m = glm::vec2(
		target_position.z - player_position.z,
		target_position.x - player_position.x
	);

	m = -m;

	//get smallest angle
	float angle = atan2(m.x, m.y) - atan2(n.x, n.y);

	if (angle > glm::pi<float>())
		angle = -glm::two_pi<float>() + angle;
	else if (angle < -glm::pi<float>()) {
		angle = glm::two_pi<float>() + angle;
	}

	//get to know if it move left or right
	float direction = 1.0f;
	angle < 0.f ? direction = -1.0f : direction = 1.0f;

	float range_angle = (range_angle_degree * glm::pi<float>() / 180.f);
	//set end angle
	if (glm::abs(angle) > range_angle)
		end_angle = (begin_angle + direction * range_angle) - angle;
	else
		end_angle = begin_angle;

	if(!curr_angle)
		curr_angle = &cd->angle;
}

void FollowCameraPhysicComponent::setBeginHorizon(float horizon)
{
	begin_horizon = horizon;
}

void FollowCameraPhysicComponent::setEndHorizon(float horizon, VoxelTerrainData* vt)
{
	end_horizon = horizon;

	if (!curr_horizon)
		curr_horizon = &vt->horizon;
}

