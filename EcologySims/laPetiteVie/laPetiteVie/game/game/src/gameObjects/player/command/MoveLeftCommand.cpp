#include "MoveLeftCommand.h"
#include <iostream>
MoveLeftCommand::MoveLeftCommand()
{
    //ctor
}

MoveLeftCommand::~MoveLeftCommand()
{
    //dtor
}

void MoveLeftCommand::execute(GameObject &parent, Scene &scene, const sf::Time& dt)
{
	const double pi = 3.14159265358979323846;

	PlayerData* player_data = parent.getComponent<PlayerData>(COMPONENT_DATA::PLAYER);
	Transform* player_transform = parent.getComponent<Transform>(COMPONENT_DATA::TRANSFORM);
    physicBodyData* phy = parent.getComponent<physicBodyData>(COMPONENT_DATA::PHYSIC_BODY);


	float angle_camera{ 0 };
	GameObject* cam = scene.get_child("camera");
	if (cam)
		angle_camera = cam->getComponent<CameraData>(COMPONENT_DATA::CAMERA)->angle;

	GameObject* target = scene.get_child(player_data->target_name);
	glm::vec3 n;

	if (player_data->target_name != "" && target) {
		if (target) {
			glm::vec3 target_position = target->getComponent<Transform>(COMPONENT_DATA::TRANSFORM)->position;
			n = player_transform->position - target_position;
			n = -glm::normalize(glm::vec3(-n.z, 0.f, n.x));

		}
	}
	else {
		n = glm::normalize(glm::vec3(sin(angle_camera + pi / 2), 0.f, cos(angle_camera + pi / 2)));
	}

	phy->direction += -n;
	
	phy->force = 2.f;

}

void MoveLeftCommand::undo(GameObject &parent, Scene &scene, const sf::Time& dt)
{
	PlayerData* data = parent.getComponent<PlayerData>(COMPONENT_DATA::PLAYER);
	physicBodyData* phy = parent.getComponent<physicBodyData>(COMPONENT_DATA::PHYSIC_BODY);

	float angle_camera{ 0 };
	GameObject* cam = scene.get_child("camera");
	if (cam)
		angle_camera = cam->getComponent<CameraData>(COMPONENT_DATA::CAMERA)->angle;

	const double pi = 3.14159265358979323846;
	glm::vec3 n = glm::normalize(glm::vec3(sin(angle_camera + pi / 2), 0.f, cos(angle_camera + pi / 2)));

	phy->direction += n;
	phy->force = 2.f;
} 

