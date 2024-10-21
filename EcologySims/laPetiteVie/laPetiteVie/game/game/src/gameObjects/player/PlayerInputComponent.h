#pragma once

#include <thsan/gameObject/component/InputComponent.h>

class MoveDownCommand;
class MoveLeftCommand;
class MoveRightCommand;
class MoveUpCommand;
class RotateLeftCommand;
class RotateRightCommand;
class jumpCommand;
class ActionCommand;
class RunCommand;
class StopRunCommand;
class AimCommand;
class StopAimCommand;

class PlayerInputComponent : public InputComponent
{
    public:
        PlayerInputComponent();

        void init(Scene& scene) override;
        void update(Scene& scene, const sf::Time& dt, std::vector<Config::InputAction> inputActions) override;
        virtual std::vector<Command*> handleInput(std::vector<Config::InputAction> inputActions) override;

        ~PlayerInputComponent();

    private:
        Command* aim;
        Command* stopAim;
        Command* left;
        Command* down;
        Command* up;
        Command* right;
        Command* jump;
        Command* rotateLeft;
        Command* rotateRight;
        Command* run;
        Command* stop_run;
        Command* action;
        std::vector<Command*> historyInput;

        PlayerData* player_data;
        CharacterStatData* character_stat_data;
};

