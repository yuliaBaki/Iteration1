#pragma once

#include <Thsan.h>
#include <iostream>

#include "MapState.h"

class Map0000 : public MapState
{
public:
    Map0000(Game* parent);
    void init() override;
    void handleEvent(sf::Event event) override;
    void input(const sf::Time& deltaTime, std::vector<Config::InputAction> inputActions) override;
    void update(const sf::Time& deltaTime) override;
    void draw(sf::RenderTarget& target, const sf::Time& deltaTime) override;

    ~Map0000() = default;

private:
    std::vector<std::shared_ptr<State>> decorative;
};

