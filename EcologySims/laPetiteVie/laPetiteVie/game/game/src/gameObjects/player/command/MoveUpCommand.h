#ifndef MOVEUPCOMMAND_H
#define MOVEUPCOMMAND_H

#include <thsan/Input/Command.h>


class MoveUpCommand : public Command
{
    public:
        MoveUpCommand();
        ~MoveUpCommand();

        void execute(GameObject &parent, Scene &scene, const sf::Time& dt) override;
        void undo(GameObject &parent, Scene &scene, const sf::Time& dt) override;
};

#endif // MOVEUPCOMMAND_H
