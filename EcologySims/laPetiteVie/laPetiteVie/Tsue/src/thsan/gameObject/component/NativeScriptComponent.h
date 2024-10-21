#pragma once

#include "../GameObject.h"

class GameObject;

class NativeScriptComponent
{
public:
    NativeScriptComponent() = default;
    virtual ~NativeScriptComponent() = default;

    virtual void init(Scene& scene) {};
    virtual void update(Scene& scene, const sf::Time& dt) {};
    virtual void onDelete(Scene& scene) {};

    bool isActive() { return enabled; }
    void disable() { enabled = false; }
    void enable() { enabled = true; }

protected:
    GameObject* parent;

    friend GameObject;
    friend Scene;
private:
    bool enabled{ true };
};

