#include "StopAimCommand.h"

StopAimCommand::StopAimCommand()
{
}

void StopAimCommand::execute(GameObject& parent, Scene& scene, const sf::Time& dt)
{
	GameObject* bars = scene.get_child("bars");

	PlayerData* player = parent.getComponent<PlayerData>(COMPONENT_DATA::PLAYER);
	if (player->target_name != "")
		player->target_name = "";

	player->isAiming = false;


	if (!bars)
		return;

	if (bars->hasComponent(COMPONENT_DATA::BARS))
		bars->getComponent<BarsData>(COMPONENT_DATA::BARS)->show_bar = false;
}

void StopAimCommand::undo(GameObject& parent, Scene& scene, const sf::Time& dt)
{

}
