#include "MainMenuState.h"


#include <thsan/UI/LongText.h>
#include <thsan/UI/LabelButton.h>
#include <thsan/UI/Panel.h>
#include <thsan/ressourceManager/FontManager.h>
#include <thsan/ressourceManager/StateManager.h>

#include "../dialogue/DialogueState.h"
#include "../map/Map0000.h"



MainMenuState::MainMenuState(Game* parent) :
	State(parent)
{
    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
    for (std::size_t i = 0; i < modes.size(); ++i)
    {
        sf::VideoMode mode = modes[i];
        std::cout << "Mode #" << i << ": "
                  << mode.width << "x" << mode.height << " - "
                  << mode.bitsPerPixel << " bpp" << std::endl;
    }
}

std::string MainMenuState::getType()
{
    return "mainMenu";
}

void MainMenuState::init()
{
  m_parent->changeState( RessourceManager::StateManager::create<Map0000>(m_parent), true);
}

void MainMenuState::handleEvent(sf::Event event)
{

}

void MainMenuState::input(const sf::Time &deltaTime, std::vector<Config::InputAction> inputActions){

}

void MainMenuState::update(const sf::Time &deltaTime)
{

}

void MainMenuState::draw(sf::RenderTarget& target, const sf::Time& deltaTime)
{
}



