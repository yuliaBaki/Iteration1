#include "CameraInputComponent.h"


CameraInputComponent::CameraInputComponent()
{
}


void CameraInputComponent::init(Scene& scene)
{

}

void CameraInputComponent::update(Scene& scene,  const sf::Time& dt, std::vector<Config::InputAction> inputActions)
{

}

std::vector<Command*> CameraInputComponent::handleInput(std::vector<Config::InputAction> inputActions)
{
	return std::vector<Command*>();
}

CameraInputComponent::~CameraInputComponent()
{
}
