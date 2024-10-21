#pragma once

#include <thsan/Input/Command.h>

class RotateRightCommand : public Command
{
public:
    RotateRightCommand();
    ~RotateRightCommand();

    void execute(GameObject& parent, Scene& scene, const sf::Time& dt) override;
    void undo(GameObject& parent, Scene& scene, const sf::Time& dt) override;
};