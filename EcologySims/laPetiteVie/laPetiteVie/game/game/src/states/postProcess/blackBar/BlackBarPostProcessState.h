#pragma once
#include <Thsan.h>

#include <iostream>

class BlackBarPostProcessState : public State
{
public:
    BlackBarPostProcessState(
        Game* parent,
        std::shared_ptr<State> previous,
        Scene* scene,
        float bar_scren_ratio_top = 0.1f,
        float bar_scren_ratio_bottom = 0.1f,
        sf::Time duration = sf::seconds(1.0f),
        float end_speed = 3.f ,
        sf::Color top_bar_color = sf::Color::Black,
        sf::Color bottom_bar_color = sf::Color::Black,
        bool horizontal = true);
    std::string getType() override;
    void init() override;
    void handleEvent(sf::Event event) override;
    void input(const sf::Time& deltaTime, std::vector<Config::InputAction> inputActions) override;
    void update(const sf::Time& deltaTime) override;
    void draw(sf::RenderTarget& target, const sf::Time& deltaTime) override;

    void hide_bar();
    void show_bar();

    ~BlackBarPostProcessState() = default;

private:
    float bar_screen_ratio_top{0.1f};
    float bar_screen_ratio_bottom{0.1f};
    float curr_bar_screen_ratio_top{0.0f};
    float curr_bar_screen_ratio_bottom{0.0f};
    sf::Time duration{ sf::seconds(0.f) };
    float end_speed{ 3.f };
    sf::Color top_bar_color{ sf::Color::Black };
    sf::Color bottom_bar_color{ sf::Color::Black };
    sf::Time elapsedTime{ sf::seconds(0.f) };

    bool enable_start{ false };
    bool enable_end{ false };

    sf::RenderTexture texture;
    sf::Texture frame_texture;
    sf::RenderWindow* window;
    std::shared_ptr<State> target_state;
    Scene* scene;
    sf::Sprite s;
    std::shared_ptr<sf::Shader> shader_pp;
};

