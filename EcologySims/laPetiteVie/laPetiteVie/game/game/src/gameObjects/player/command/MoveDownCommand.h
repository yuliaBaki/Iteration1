#ifndef MOVEDOWNCOMMAND_H
#define MOVEDOWNCOMMAND_H

#include <thsan/Input/Command.h>

class MoveDownCommand : public Command
{
    public:
        MoveDownCommand();
        ~MoveDownCommand();

        void execute(GameObject &parent, Scene &scene, const sf::Time& dt) override;
        void undo(GameObject &parent, Scene &scene, const sf::Time& dt) override;
};

#endif // MOVEDOWNCOMMAND_H
