#include "FightDemo.h"

#include "../../gameObjects/player/PlayerInputComponent.h"
#include "../../gameObjects/player/PlayerPhysicComponent.h"
#include "../../gameObjects/player/PlayerSpriteRenderComponent.h"


#include <thsan/UI/LongText.h>
#include <thsan/UI/LabelButton.h>
#include <thsan/UI/Panel.h>
#include <thsan/ressourceManager/FontManager.h>
#include <thsan/ressourceManager/TextureManager.h>

#include "../dialogue/DialogueState.h"

FightDemo::FightDemo(Game* parent):
	State(parent),
	m_scene(this)
{
    m_scene.setRenderTarget(&parent->getWindow());

    m_scene.createGameObject(new PlayerInputComponent(), new PlayerPhysicComponent(), new PlayerSpriteRenderComponent(), "player");
    m_scene.createGameObject(new PlayerInputComponent(), new PlayerPhysicComponent(), new PlayerSpriteRenderComponent(), "player2");
    m_scene.createTerrainGameObject();

    m_scene.get_child("player2")->getComponent<Transform>(COMPONENT_DATA::TRANSFORM)->position.y = 0.0f;
    m_scene.get_child("player2")->getComponent<Transform>(COMPONENT_DATA::TRANSFORM)->position.x = -60.0f;
    /*
    m_scene.setDirLight_diffuse(sf::Color(150, 30, 200));
    m_scene.setDirLight_ambient(sf::Color(30, 0, 50));
    m_scene.setDirLight_direction(sf::Vector3f(1, 0, 0.0));
    */

    m_scene.setDirLight_diffuse(sf::Color(255, 0, 0));
    m_scene.setDirLight_ambient(sf::Color(0, 0, 0));

}

void FightDemo::init()
{
}

void FightDemo::handleEvent(sf::Event event)
{

}

void FightDemo::input(const sf::Time& deltaTime, std::vector<Config::InputAction> inputActions)
{
    m_scene.input(deltaTime, inputActions);
}

void FightDemo::update(const sf::Time& deltaTime)
{
    m_scene.update(deltaTime);
    static float temps = 0;
    temps += deltaTime.asSeconds();

    m_scene.setDirLight_direction(sf::Vector3f(cos(temps), 1, sin(temps)));
    //m_scene.get_child("player2")->getComponent<Transform>(COMPONENT_DATA::TRANSFORM)->position.z = -30.0f;

}

void FightDemo::draw(sf::RenderTarget& target, const sf::Time& deltaTime)
{
    LightProperties dir = m_scene.getDirectionalLightProperties();
    target.clear(dir.ambient);
    m_scene.render(deltaTime);
}
