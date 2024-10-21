#ifndef MOVELEFTCOMMAND_H
#define MOVELEFTCOMMAND_H

#include <thsan/Input/Command.h>

class MoveLeftCommand : public Command
{
    public:
        MoveLeftCommand();
        ~MoveLeftCommand();

        void execute(GameObject &parent, Scene &scene, const sf::Time& dt) override;
        void undo(GameObject &parent, Scene &scene, const sf::Time& dt) override;
};

#endif // MOVELEFTCOMMAND_H
