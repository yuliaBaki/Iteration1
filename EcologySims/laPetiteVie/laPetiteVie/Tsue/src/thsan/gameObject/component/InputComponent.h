#pragma once

#include <SFML/System/Time.hpp>

#include "../GameObject.h"
#include "../../Input/Command.h"
#include "../../config/ControlSetting.h"

class GameObject;
class Scene;
class Command;

class InputComponent
{
public:
    InputComponent(){};
    virtual void init(Scene& scene){}
    virtual void update(Scene& scene, const sf::Time& dt, std::vector<Config::InputAction> inputActions){}
    virtual void onDelete(Scene& scene) {};
    virtual std::vector<Command*> handleInput(std::vector<Config::InputAction> inputActions){ return std::vector<Command*>();}
    virtual ~InputComponent();

    void undo(Scene& scene, const sf::Time& dt);
    void redo(Scene& scene, const sf::Time& dt);
    bool isActive() { return enabled; }
    void disable() { enabled = false; }
    void enable() { enabled = true; }
protected:
    GameObject* parent;
    std::vector<Command*> commandHistory;
	int currCommand{0};

    friend Scene;
    friend GameObject;
private:
    bool enabled{ true };
};

