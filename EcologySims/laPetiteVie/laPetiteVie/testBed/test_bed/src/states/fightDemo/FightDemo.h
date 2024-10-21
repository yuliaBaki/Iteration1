#pragma once

#include <thsan/state/State.h>
#include <thsan/scene/Scene.h>
#include <thsan/UI/Gui.h>
#include  <thsan/UI/LongText.h>
#include  <thsan/UI/Panel.h>
#include  <thsan/UI/LabelButton.h>

#include <iostream>

class FightDemo : public State
{
public:
    FightDemo(Game* parent);
    void init() override;
    void handleEvent(sf::Event event) override;
    void input(const sf::Time& deltaTime, std::vector<Config::InputAction> inputActions) override;
    void update(const sf::Time& deltaTime) override;
    void draw(sf::RenderTarget& target, const sf::Time& deltaTime) override;

    ~FightDemo() = default;
private:
    Scene m_scene;
};
