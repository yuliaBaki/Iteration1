#include "PlayerInputComponent.h"
#include <thsan/ressourceManager/StateManager.h>
#include "../../states/postProcess/blackBar/BlackBarPostProcessState.h"
#include "command/MoveDownCommand.h"
#include "command/MoveLeftCommand.h"
#include "command/MoveRightCommand.h"
#include "command/MoveUpCommand.h"
#include "command/RotateLeftCommand.h"
#include "command/RotateRightCommand.h"
#include "command/jumpCommand.h"
#include "command/ActionCommand.h"
#include "command/RunCommand.h"
#include "command/StopRunCommand.h"
#include "command/AimCommand.h"
#include "command/StopAimCommand.h"

PlayerInputComponent::PlayerInputComponent()
{
}


void PlayerInputComponent::init(Scene& scene)
{
	left = new MoveLeftCommand();
	down = new MoveDownCommand();
	up = new MoveUpCommand();
	right = new MoveRightCommand();
	jump = new JumpCommand();
	rotateLeft = new RotateLeftCommand();
	rotateRight = new RotateRightCommand();
	run = new RunCommand();
	stop_run = new StopRunCommand();
	action = new ActionCommand();
	aim = new AimCommand();
	stopAim = new StopAimCommand();

	if (!parent->hasComponent(COMPONENT_DATA::PLAYER)) {
		player_data = new PlayerData();
		parent->setComponent(COMPONENT_DATA::PLAYER, player_data);
	}
	else {
		player_data = parent->getComponent<PlayerData>(COMPONENT_DATA::PLAYER);
	}

	if (!parent->hasComponent(COMPONENT_DATA::CHARACTER_STAT)) {
		character_stat_data = new CharacterStatData();
		parent->setComponent(COMPONENT_DATA::CHARACTER_STAT, character_stat_data);
	}
	else {
		character_stat_data = parent->getComponent<CharacterStatData>(COMPONENT_DATA::CHARACTER_STAT);
	}

	player_data->curr_state = PlayerData::State::idle;

	//load from BD...DTO
	character_stat_data->hp = 100;
	character_stat_data->mp = 10;
	character_stat_data->curr.hp = 75;
	character_stat_data->curr.mp = 5;
}

void PlayerInputComponent::update(Scene& scene,  const sf::Time& dt, std::vector<Config::InputAction> inputActions)
{
	physicBodyData* phy = parent->getComponent<physicBodyData>(COMPONENT_DATA::PHYSIC_BODY);

	if(phy)
		phy->direction = glm::vec3(0, 0, 0);

    std::vector<Command*> commands = handleInput(inputActions);

	for (auto c : commands) {
		c->execute(*parent, scene, dt);
		historyInput.push_back(c);
	}
	
	commands.clear();
}

std::vector<Command*> PlayerInputComponent::handleInput(std::vector<Config::InputAction> inputActions)
{
	std::vector<Command*> commands;
	
	for (Config::InputAction inputAction : inputActions)
	{
		if (inputAction == Config::InputAction::up)
			commands.push_back(up);
		if (inputAction == Config::InputAction::down)
			commands.push_back(down);
		if (inputAction == Config::InputAction::left)
			commands.push_back(left);
		if (inputAction == Config::InputAction::right)
			commands.push_back(right);
		if (inputAction == Config::InputAction::jump)
			commands.push_back(jump);
		if (inputAction == Config::InputAction::rotateLeft)
			commands.push_back(rotateLeft);
		if (inputAction == Config::InputAction::rotateRight)
			commands.push_back(rotateRight);
		if (inputAction == Config::InputAction::run)
			commands.push_back(run);
		if (inputAction == Config::InputAction::stopRun)
			commands.push_back(stop_run);
		if (inputAction == Config::InputAction::action)
			commands.push_back(action);
		if (inputAction == Config::InputAction::aim)
			commands.push_back(aim);
		if (inputAction == Config::InputAction::stopAim)
			commands.push_back(stopAim);
	}

	return commands;
}

PlayerInputComponent::~PlayerInputComponent()
{
	delete left;
	delete down;
	delete up;
	delete right;
	delete jump;
	delete rotateLeft;
	delete rotateRight;
	delete action;
	delete aim;
	delete stopAim;
}
