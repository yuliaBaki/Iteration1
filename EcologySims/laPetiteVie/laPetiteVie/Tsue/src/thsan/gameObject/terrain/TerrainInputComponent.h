#pragma once

#include "../component/InputComponent.h"


class TerrainInputComponent : public InputComponent
{
    public:
        TerrainInputComponent();

        void init(Scene& scene) override;
        void update(Scene& scene, const sf::Time& dt, std::vector<Config::InputAction> inputActions) override;
        virtual std::vector<Command*> handleInput(std::vector<Config::InputAction> inputActions) override;

        ~TerrainInputComponent();

    private:

};
