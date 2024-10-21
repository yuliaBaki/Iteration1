#include "SimpleDialogNPCInputComponent.h"
#include "../../states/dialogue/DialogueState.h"

#include <iostream>

SimpleDialogNPCInputComponent::SimpleDialogNPCInputComponent()
{
}


void SimpleDialogNPCInputComponent::init(Scene& scene)
{
	if(scene.get_child("player"))
		playerTransform = scene.get_child("player")->getComponent<Transform>(COMPONENT_DATA::TRANSFORM);

	transform = parent->getComponent<Transform>(COMPONENT_DATA::TRANSFORM);
}

void SimpleDialogNPCInputComponent::update(Scene& scene,  const sf::Time& dt, std::vector<Config::InputAction> inputActions)
{

    std::vector<Command*> commands = handleInput(inputActions);

	for (auto c : commands) {
		c->execute(*parent, scene, dt);
		historyInput.push_back(c);
	}
	
	commands.clear();
}

std::vector<Command*> SimpleDialogNPCInputComponent::handleInput(std::vector<Config::InputAction> inputActions)
{
	std::vector<Command*> commands;
	glm::vec3 temp = playerTransform->position - transform->position;
	float distance = sqrt(temp.x*temp.x + 20 + temp.z*temp.z);
	for (Config::InputAction inputAction : inputActions)
		if (inputAction == Config::InputAction::select && distance < 5)
			;//state_parent->getParent()->pushState(new DialogueState(parent, state_parent, sf::Vector2f(10, 600), sf::Vector2f(100, 200),  ))


	return commands;

}

SimpleDialogNPCInputComponent::~SimpleDialogNPCInputComponent()
{
}
