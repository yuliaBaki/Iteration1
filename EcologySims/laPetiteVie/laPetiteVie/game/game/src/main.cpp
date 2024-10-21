#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <Thsan.h>
#include "states/mainMenu/MainMenuState.h"

int main()
{
    srand(time(NULL));
    Config::ControlSetting controlSetting;
    controlSetting.add(sf::Keyboard::A, Config::InputState::isPressed, Config::InputAction::left);
    controlSetting.add(sf::Keyboard::S, Config::InputState::isPressed, Config::InputAction::down);
    controlSetting.add(sf::Keyboard::W, Config::InputState::isPressed, Config::InputAction::up);
    controlSetting.add(sf::Keyboard::D, Config::InputState::isPressed, Config::InputAction::right);
    controlSetting.add(sf::Keyboard::P, Config::InputState::isPressed, Config::InputAction::run);
    controlSetting.add(sf::Keyboard::P, Config::InputState::isReleased, Config::InputAction::stopRun);
    controlSetting.add(sf::Keyboard::E, Config::InputState::isPressed, Config::InputAction::rotateLeft);
    controlSetting.add(sf::Keyboard::Q, Config::InputState::isPressed, Config::InputAction::rotateRight);
    controlSetting.add(sf::Keyboard::O, Config::InputState::isPressed, Config::InputAction::aim);
    controlSetting.add(sf::Keyboard::O, Config::InputState::isReleased, Config::InputAction::stopAim);
    controlSetting.add(sf::Keyboard::Space, Config::InputState::isPressedNoRepeat, Config::InputAction::jump);
    controlSetting.add(sf::Keyboard::Enter, Config::InputState::isPressedNoRepeat, Config::InputAction::select);
    controlSetting.add(sf::Keyboard::L, Config::InputState::isPressedNoRepeat, Config::InputAction::action);

    Game game(1024, 768);
    game.changeState(RessourceManager::StateManager::create<MainMenuState>(&game), true);
    game.setControlSettings(&controlSetting);
    game.run();

    return 0;
}
