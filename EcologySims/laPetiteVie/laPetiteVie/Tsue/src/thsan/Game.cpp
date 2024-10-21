#include "Game.h"
#include <sstream>
#include <iostream>
#include "ressourceManager/ShaderManager.h"
#include "ressourceManager/StateManager.h"

Game::Game(int width, int height):
    window(sf::VideoMode(width, height), "PP", sf::Style::Default, sf::ContextSettings(0, 0, 0, 4, 3, 0, false))
{
    window.setActive(false);
    //window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);
    window.setVerticalSyncEnabled(false);
    RessourceManager::ShaderManager::get("media/shader/terrain/terrain.vert", "media/shader/terrain/terrain.frag");

    font.loadFromFile("media/font/advanced_pixel-7.ttf");
    fps_text.setFont(font);
}

int Game::getWidth() const
{
    return window.getSize().x;
}

int Game::getHeight() const
{
    return window.getSize().y;
}

sf::RenderWindow& Game::getWindow()
{
    return window;
}

void Game::changeState(std::shared_ptr<State> state, bool doesItRequireInit)
{
    if (state) {

        // if it passes by a transitionState where the constructor is created in parrallel,
        // it won't be added in the ressourceManager. This if statement kinda ensure it work.
        // TransitionState cannot include ressourceManager itself, it would create a 
        // circular dependency and ressourceManager doesn't simply use the transitionState
        // pointer. This is clearly a design mistake and a hack. A better solution has to be provided
        // as of now, my IQ is to low to think of anything.

        if (!RessourceManager::StateManager::there_exists(state->getId())) 
        {
            RessourceManager::StateManager::add(state);
        }

        this->state = state;
        if (doesItRequireInit) {
            state->init();
            state->update(sf::Time::Zero);
            sf::RenderTexture temp_rt;
            temp_rt.create(1, 1);
            state->draw(temp_rt, sf::Time::Zero);
        }
        RessourceManager::StateManager::removeUnused();
    }
}

std::shared_ptr<State> Game::peekState()
{
    return this->state;
}

void Game::setControlSettings(Config::ControlSetting* controlSetting)
{
    this->controlSetting = controlSetting;
}

sf::ContextSettings Game::getContextSettings()
{
    return settings;
}

void Game::run()
{
    //gestion du temps
    sf::Time timePerFrame = sf::seconds(1.f / 60.f);


    static sf::Clock clock;
    static sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while(state){

        sf::Time latest = clock.restart();
        timeSinceLastUpdate += latest;

        if (state) {

            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                {
                    state = nullptr;
                    window.close();
                }
                else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Equal) {
                    std::stringstream filename;
                    filename << "screenshot_1.png";
                    sf::Texture texture;
                    texture.create(window.getSize().x, window.getSize().y);
                    texture.update(window);
                    if (texture.copyToImage().saveToFile("screenshot_1.png"))
                    {
                        std::cout << "screenshot saved to " << std::endl;
                    }
                }
                else {
                    controlSetting->handleInput(event);
                    state->handleEvent(event);
                }
            }

            //std::cout << timeSinceLastUpdate.asSeconds() << "\n";
            std::string s = std::to_string(1.0 / latest.asSeconds());
            fps_text.setString(s);
        }

        //while(timeSinceLastUpdate > timePerFrame){
             if (state) {
                state->update(latest);
                controlSetting->updateInput();
                state->input(latest, controlSetting->getInput());

				//timeSinceLastUpdate -= timePerFrame;
                state->draw(window, latest);

                if (window.isOpen()) {
                    fps_text.setCharacterSize(20);
                    fps_text.setPosition(sf::Vector2f(20, 20));
                    fps_text.setFillColor(sf::Color::Cyan);
                    sf::Shader::bind(0);
                    window.draw(fps_text);
                }
            }
        //}
		
        window.display();
    }
}


