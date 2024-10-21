#pragma once

#include <Thsan.h>
#include <iostream>

class MapState : public State
{
public:
    MapState(Game* parent);
    std::string getType() override;
    void init() = 0;
    void handleEvent(sf::Event event) = 0;
    void input(const sf::Time& deltaTime, std::vector<Config::InputAction> inputActions) = 0;
    void update(const sf::Time& deltaTime) = 0;
    void draw(sf::RenderTarget& target, const sf::Time& deltaTime) = 0;

    ~MapState();

protected:
    Scene* m_scene;

    friend class BattleState;
};

