#include "AimCommand.h"
#include <iostream>
#include <thsan/CollisionEngine/CollisionEngine.h>
#include <unordered_set>

AimCommand::AimCommand()
{
}

void AimCommand::execute(GameObject& parent, Scene& scene, const sf::Time& dt)
{
	GameObject* bars = scene.get_child("bars");

	if (parent.hasComponent(COMPONENT_DATA::PLAYER)) {
		PlayerData* player = parent.getComponent<PlayerData>(COMPONENT_DATA::PLAYER);
		if (player->target_name == "")
		{
			GameObject* go = CollisionEngine::getOneClosestGameObjectWithinRadius(parent.getComponent<Transform>(COMPONENT_DATA::TRANSFORM), 100, -100, 100, Collision_object::real);
			if (go) {
				player->target_transform = go->getComponent<Transform>(COMPONENT_DATA::TRANSFORM);
				player->target_name = go->getName();

				player->isAiming = true;
				player->curr_state = PlayerData::State::charge;

				if (!bars)
					return;

				if (bars->hasComponent(COMPONENT_DATA::BARS))
					bars->getComponent<BarsData>(COMPONENT_DATA::BARS)->show_bar = true;
			}
		}
	}
}

void AimCommand::undo(GameObject& parent, Scene& scene, const sf::Time& dt)
{
}

void AimCommand::show_bar()
{
}

void AimCommand::hide_bar()
{
}
