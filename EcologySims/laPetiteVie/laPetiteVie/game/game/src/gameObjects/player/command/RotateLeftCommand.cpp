#include "RotateLeftCommand.h"
#include <iostream>
RotateLeftCommand::RotateLeftCommand()
{
	//ctor
}

RotateLeftCommand::~RotateLeftCommand()
{
	//dtor
}

void RotateLeftCommand::execute(GameObject& parent, Scene& scene, const sf::Time& dt)
{
	GameObject* go = scene.get_child("camera");
	CameraData* camera_data = go->getComponent<CameraData>(COMPONENT_DATA::CAMERA);
	PlayerData* player_data = parent.getComponent<PlayerData>(COMPONENT_DATA::PLAYER);

	if (player_data->target_name == "" && scene.pseudo3DRendering_getViewType() == VIEW_TYPE::DYNAMIC_VIEW)
		camera_data->angle += dt.asSeconds() * 3.f;
}

void RotateLeftCommand::undo(GameObject& parent, Scene& scene, const sf::Time& dt)
{
	GameObject* go = scene.get_child("camera");
	CameraData* camera_data = go->getComponent<CameraData>(COMPONENT_DATA::CAMERA);
	PlayerData* player_data = parent.getComponent<PlayerData>(COMPONENT_DATA::PLAYER);

	if (player_data->target_name == "" && scene.pseudo3DRendering_getViewType() == VIEW_TYPE::DYNAMIC_VIEW)
		camera_data->angle -= dt.asSeconds() * 3.f;
}

