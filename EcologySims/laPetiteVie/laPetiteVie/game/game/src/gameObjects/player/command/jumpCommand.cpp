#include "jumpCommand.h"

JumpCommand::JumpCommand()
{
}

void JumpCommand::execute(GameObject& parent, Scene& scene, const sf::Time& dt)
{
	if (!parent.hasComponent(COMPONENT_DATA::PHYSIC_BODY))
		return;

	physicBodyData* phy = parent.getComponent<physicBodyData>(COMPONENT_DATA::PHYSIC_BODY);

	if (phy->onGround) {
		phy->isJump = true;
		phy->onGround = false;
	}
	else {
		phy->isDoubleJump = true;
	}

}

void JumpCommand::undo(GameObject& parent, Scene& scene, const sf::Time& dt)
{
}
