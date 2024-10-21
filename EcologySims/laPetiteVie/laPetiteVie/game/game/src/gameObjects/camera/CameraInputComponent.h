#ifndef CAMERAINPUTCOMPONENT_H
#define CAMERAINPUTCOMPONENT_H

#include <thsan/gameObject/component/InputComponent.h>


class CameraInputComponent : public InputComponent
{
    public:
		CameraInputComponent();

        void init(Scene& scene) override;
        void update(Scene& scene, const sf::Time& dt, std::vector<Config::InputAction> inputActions) override;
        virtual std::vector<Command*> handleInput(std::vector<Config::InputAction> inputActions) override;

        ~CameraInputComponent();

    private:
};

#endif // PLAYERINPUTCOMPONENT_H
