#pragma once

#include <thsan/Input/Command.h>

class ActionCommand : public Command
{
public:
    ActionCommand();
    ~ActionCommand() = default;

    void execute(GameObject& parent, Scene& scene, const sf::Time& dt) override;
    void undo(GameObject& parent, Scene& scene, const sf::Time& dt) override;
private:

};