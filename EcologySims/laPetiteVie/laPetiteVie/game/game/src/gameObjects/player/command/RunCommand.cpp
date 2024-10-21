#include "runCommand.h"

RunCommand::RunCommand()
{
}

void RunCommand::execute(GameObject& parent, Scene& scene, const sf::Time& dt)
{
	PlayerData* playerData{nullptr};
	if (parent.hasComponent(COMPONENT_DATA::PLAYER)) {
		playerData = parent.getComponent<PlayerData>(COMPONENT_DATA::PLAYER);
		if(playerData->curr_state == PlayerData::State::walk)
			playerData->curr_state = PlayerData::State::run;
	}
}

void RunCommand::undo(GameObject& parent, Scene& scene, const sf::Time& dt)
{
	PlayerData* playerData{ nullptr };
	if (parent.hasComponent(COMPONENT_DATA::PLAYER)) {
		playerData = parent.getComponent<PlayerData>(COMPONENT_DATA::PLAYER);
		if (playerData->curr_state == PlayerData::State::run)
			playerData->curr_state = PlayerData::State::walk;
	}
}
