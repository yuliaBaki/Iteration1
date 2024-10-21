#include "MainMenuState.h"

#include <thsan/UI/LongText.h>
#include <thsan/UI/LabelButton.h>
#include <thsan/UI/Panel.h>
#include <thsan/ressourceManager/FontManager.h>


MainMenuState::MainMenuState(Game* parent) :
	State(parent),
	m_scene(this)
{
    m_scene.setRenderTarget(&parent->getWindow());

    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
    for (std::size_t i = 0; i < modes.size(); ++i)
    {
        sf::VideoMode mode = modes[i];
        std::cout << "Mode #" << i << ": "
                  << mode.width << "x" << mode.height << " - "
                  << mode.bitsPerPixel << " bpp" << std::endl;
    }


}

void MainMenuState::init()
{
}

void MainMenuState::handleEvent(sf::Event event)
{

}

void MainMenuState::input(const sf::Time &deltaTime, std::vector<Config::InputAction> inputActions){

    m_scene.input(deltaTime, inputActions);

}

void MainMenuState::update(const sf::Time &deltaTime)
{
    m_scene.update(deltaTime);
	static float temps = 0;
	temps += deltaTime.asSeconds();
}

void MainMenuState::draw(sf::RenderTarget& target, const sf::Time& deltaTime)
{
    LightProperties dir = m_scene.getDirectionalLightProperties();
    target.clear(dir.ambient);
    m_scene.render(deltaTime);
}



