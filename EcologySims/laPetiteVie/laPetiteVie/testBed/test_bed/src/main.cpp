#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <Thsan.h>
#include "states/editor/EditorState.h"
#include <thsan/ressourceManager/StateManager.h>

int main()
{
    srand(time(NULL));
    Config::ControlSetting controlSetting;
    controlSetting.add(sf::Keyboard::A, Config::InputState::isPressed, Config::InputAction::left);
    controlSetting.add(sf::Keyboard::S, Config::InputState::isPressed, Config::InputAction::down);
    controlSetting.add(sf::Keyboard::W, Config::InputState::isPressed, Config::InputAction::up);
    controlSetting.add(sf::Keyboard::D, Config::InputState::isPressed, Config::InputAction::right);
    controlSetting.add(sf::Keyboard::Enter, Config::InputState::isPressedNoRepeat, Config::InputAction::select);
    controlSetting.add(sf::Keyboard::Enter, Config::InputState::isPressedNoRepeat, Config::InputAction::action);

    Game game(1366, 768);
    std::shared_ptr<State> pp = RessourceManager::StateManager::create<EditorState>(&game);
    game.changeState(pp, true);
    game.setControlSettings(&controlSetting);
    game.run();
    return 0;
}
