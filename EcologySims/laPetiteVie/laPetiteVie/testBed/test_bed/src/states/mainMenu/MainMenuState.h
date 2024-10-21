#ifndef MAINMENU_H
#define MAINMENU_H

#include <Thsan.h>


#include <iostream>

class MainMenuState : public State
{
    public:
        MainMenuState(Game* parent);
        void init() override;
        void handleEvent(sf::Event event) override;
        void input(const sf::Time &deltaTime, std::vector<Config::InputAction> inputActions) override;
        void update(const sf::Time &deltaTime) override;
        void draw(sf::RenderTarget& target, const sf::Time &deltaTime) override;

        ~MainMenuState() = default;

    private:

        Scene m_scene;
};

#endif // MAINMENU_H
