#ifndef MOVERIGHTCOMMAND_H
#define MOVERIGHTCOMMAND_H

#include <thsan/Input/Command.h>

class MoveRightCommand : public Command
{
    public:
        MoveRightCommand();
        ~MoveRightCommand();

        void execute(GameObject &parent, Scene &scene, const sf::Time& dt) override;
        void undo(GameObject &parent, Scene &scene, const sf::Time& dt) override;
};


#endif // MOVERIGHTCOMMAND_H
