#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "config/ControlSetting.h"
#include <stack>
#include <memory>
#include "state/State.h"


class State;

class Game
{
    public:
        Game(int width, int height);
        void run();
        int getWidth() const;
        int getHeight() const;

        sf::RenderWindow& getWindow();

        void changeState(std::shared_ptr<State> state, bool doesItRequireInit);
        std::shared_ptr<State> peekState();

        void setControlSettings(Config::ControlSetting* controlSetting);
        sf::ContextSettings getContextSettings();

    private:
        sf::RenderWindow window;

        sf::Font font;
        sf::Text fps_text;

        sf::ContextSettings settings;
		Config::ControlSetting* controlSetting;
        std::shared_ptr<State> state;
        
};

#endif // GAME_H
