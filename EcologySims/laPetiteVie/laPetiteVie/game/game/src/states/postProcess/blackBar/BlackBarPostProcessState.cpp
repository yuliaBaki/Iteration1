#include "BlackBarPostProcessState.h"
#include <thsan/ressourceManager/ShaderManager.h>
#include <glm/gtx/easing.hpp>
#include <iostream>

BlackBarPostProcessState::BlackBarPostProcessState(
    Game* parent,
    std::shared_ptr<State> previous,
    Scene* scene,
    float bar_screen_ratio_top,
    float bar_screen_ratio_bottom,
    sf::Time duration,
    float end_speed,
    sf::Color top_bar_color,
    sf::Color bottom_bar_color,
    bool horizontal) :
        State(parent),
        bar_screen_ratio_top(bar_screen_ratio_top),
        bar_screen_ratio_bottom(bar_screen_ratio_bottom),
        duration(duration),
        end_speed(end_speed),
        top_bar_color(top_bar_color),
        bottom_bar_color(bottom_bar_color)
{
    window = &parent->getWindow();
    scene->setRenderTarget(&texture);
    target_state = previous;
    texture.create(parent->getWidth(), parent->getHeight(), parent->getContextSettings());

    s.setTexture(texture.getTexture());
    shader_pp = RessourceManager::ShaderManager::get("media/shader/postProcess/blackBar.vert", "media/shader/postProcess/blackBar.frag");

    this->scene = scene;
}

std::string BlackBarPostProcessState::getType()
{
    return "postprocess_blackbar";
}

void BlackBarPostProcessState::init()
{
}

void BlackBarPostProcessState::handleEvent(sf::Event event)
{
}

void BlackBarPostProcessState::input(const sf::Time& deltaTime, std::vector<Config::InputAction> inputActions) {
    target_state->input(deltaTime, inputActions);
}

void BlackBarPostProcessState::update(const sf::Time& deltaTime)
{
    if (elapsedTime < sf::seconds(0.f))
        elapsedTime = sf::seconds(0.f);

    if (elapsedTime <= sf::seconds(0.f) && enable_end)
        enable_end = false;

    if (elapsedTime > duration && enable_start)
        enable_start = false;

    if (enable_start) {
        elapsedTime += deltaTime;
        float s = glm::elasticEaseOut<float>(elapsedTime.asSeconds() / duration.asSeconds());
        curr_bar_screen_ratio_top = bar_screen_ratio_top * s;
        curr_bar_screen_ratio_bottom = bar_screen_ratio_bottom * s;
    }

    else if (enable_end) {
        elapsedTime -= deltaTime * end_speed;
        float s = glm::linearInterpolation<float>(elapsedTime.asSeconds() / duration.asSeconds());
        curr_bar_screen_ratio_top = bar_screen_ratio_top * s;
        curr_bar_screen_ratio_bottom = bar_screen_ratio_bottom * s;
    }

    target_state->update(deltaTime);
}

void BlackBarPostProcessState::draw(sf::RenderTarget& target, const sf::Time& deltaTime)
{
    target_state->draw(texture, deltaTime);
    texture.display();
    sf::Shader::bind(&*shader_pp);
    shader_pp->setUniform("texture_depth", scene->getDepthTexture());
    shader_pp->setUniform("top_bar_color", sf::Glsl::Vec4(top_bar_color));
    shader_pp->setUniform("bottom_bar_color", sf::Glsl::Vec4(bottom_bar_color));
    shader_pp->setUniform("bar_screen_ratio_top", curr_bar_screen_ratio_top);
    shader_pp->setUniform("bar_screen_ratio_bottom", curr_bar_screen_ratio_bottom);
    shader_pp->setUniform("texture_depth", scene->getDepthTexture());
    target.draw(s);
    sf::Shader::bind(0);
}

void BlackBarPostProcessState::hide_bar()

{
    enable_start = false;
    enable_end = true;
}

void BlackBarPostProcessState::show_bar()
{
    enable_start = true;
    enable_end = false;
}
