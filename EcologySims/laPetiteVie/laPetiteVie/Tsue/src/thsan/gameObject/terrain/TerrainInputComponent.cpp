#include "TerrainInputComponent.h"

#include <iostream>

TerrainInputComponent::TerrainInputComponent()
{
}


void TerrainInputComponent::init(Scene& scene)
{

}

void TerrainInputComponent::update(Scene& scene, const sf::Time& dt, std::vector<Config::InputAction> inputActions)
{
    std::vector<Command*> commands = handleInput(inputActions);

    for(auto c: commands)
        c->execute(*parent, scene, dt);
}

std::vector<Command*> TerrainInputComponent::handleInput(std::vector<Config::InputAction> inputActions)
{
    return std::vector<Command*>();
}

TerrainInputComponent::~TerrainInputComponent()
{
    //dtor
}
