#pragma once

#include <thsan/Input/Command.h>

class RunCommand : public Command
{
public:
    RunCommand();
    ~RunCommand() = default;

    void execute(GameObject& parent, Scene& scene, const sf::Time& dt) override;
    void undo(GameObject& parent, Scene& scene, const sf::Time& dt) override;
private:

};