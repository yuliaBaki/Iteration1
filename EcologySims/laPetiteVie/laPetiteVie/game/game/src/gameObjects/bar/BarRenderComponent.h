#pragma once

#include <thsan/gameObject/component/RenderComponent.h>

class PostProcess;

class BarRenderComponent : public RenderComponent
{
public:
    BarRenderComponent();
    ~BarRenderComponent();

    void init(Scene& scene) override;
    void update(Scene& scene, const sf::Time& dt, sf::RenderTarget& target, RendererPseudo3D& renderer) override;

private:
    bool enable_start{ false };
    bool enable_end{ false };
    bool is_currently_shown{ false };

    BarsData* bars_data{ nullptr };
    sf::RectangleShape top_bar;
    sf::RectangleShape bottom_bar;
    sf::Time elapsedTime{ sf::seconds(0.f) };
};
