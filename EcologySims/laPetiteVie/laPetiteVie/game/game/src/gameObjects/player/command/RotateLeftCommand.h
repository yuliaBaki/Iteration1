#pragma once

#include <thsan/Input/Command.h>


class RotateLeftCommand : public Command
{
public:
    RotateLeftCommand();
    ~RotateLeftCommand();

    void execute(GameObject& parent, Scene& scene, const sf::Time& dt) override;
    void undo(GameObject& parent, Scene& scene, const sf::Time& dt) override;
};
