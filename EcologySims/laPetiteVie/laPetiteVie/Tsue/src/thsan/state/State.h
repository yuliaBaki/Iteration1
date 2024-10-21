#ifndef STATE_H
#define STATE_H

#include <SFML/System/Time.hpp>
#include "../Game.h"
class Game;

class  State
{
    public:
        State(Game* parent);

        std::string getId();
        virtual std::string getType() = 0;

        virtual void init() = 0;

        virtual void handleEvent(sf::Event event) = 0;
        virtual void input(const sf::Time &deltaTime, std::vector<Config::InputAction> inputActions) = 0;
        virtual void update(const sf::Time &deltaTime) = 0;
        virtual void draw(sf::RenderTarget& target, const sf::Time &deltaTime) = 0;

        virtual ~State(){};

        Game *getParent(){return m_parent;}

    protected:
        std::string id;
        Game *m_parent;
};

#endif // BASESTATE_H
