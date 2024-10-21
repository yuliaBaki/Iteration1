#pragma once

#include <thsan/Input/Command.h>

class StopRunCommand : public Command
{
public:
    StopRunCommand();
    ~StopRunCommand() = default;

    void execute(GameObject& parent, Scene& scene, const sf::Time& dt) override;
    void undo(GameObject& parent, Scene& scene, const sf::Time& dt) override;
private:

};