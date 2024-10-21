#pragma once

#include "../component/RenderComponent.h"
#include "../../graphic/light/PointLight.h"

class PointLightRenderComponent : public RenderComponent
{
public:
    PointLightRenderComponent(sf::Vector3f position = sf::Vector3f(0.f, 0.f, 0.f), float constant = 1.0, float linear = 0.7, float quadratic = 1.8, sf::Color ambient = sf::Color::White, sf::Color diffuse = sf::Color::White, sf::Color specular = sf::Color::White);
    PointLightRenderComponent(PointLight* pl);
    
    void init(Scene& scene) override;
    void update(Scene& scene, const sf::Time& dt, sf::RenderTarget& target, RendererPseudo3D& rendererPseudo3D) override;
    virtual void onDelete(Scene& scene) override;


    ~PointLightRenderComponent();
private:
    bool isLoaded;
    PointLight* pl;
    LightData* lightData;
};