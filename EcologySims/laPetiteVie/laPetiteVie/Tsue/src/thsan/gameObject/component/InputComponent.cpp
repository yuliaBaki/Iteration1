#include "InputComponent.h"

InputComponent::~InputComponent()
{
    for(Command* cmd: commandHistory)
        delete cmd;
    commandHistory.clear();
}

void InputComponent::undo(Scene& scene, const sf::Time& dt)
{
    commandHistory[currCommand]->undo(*parent, scene, dt);
    currCommand--;
}

void InputComponent::redo(Scene& scene,  const sf::Time& dt)
{
    commandHistory[currCommand]->execute(*parent, scene, dt);
    currCommand++;
}
