#pragma once

#include <thsan/gameObject/component/InputComponent.h>

class SimpleDialogNPCInputComponent : public InputComponent
{
    public:
        SimpleDialogNPCInputComponent();

        void init(Scene& scene) override;
        void update(Scene& scene, const sf::Time& dt, std::vector<Config::InputAction> inputActions) override;
        virtual std::vector<Command*> handleInput(std::vector<Config::InputAction> inputActions) override;

        ~SimpleDialogNPCInputComponent();

    private:

        std::vector<Command*> historyInput;
        State* state_parent;
        Transform* playerTransform;
        Transform* transform;
};

