#pragma once

#include <thsan/gameObject/component/RenderComponent.h>
#include <thsan/graphic/SpriteAnimation.h>
#include <thsan/graphic/light/PointLight.h>

class PlayerSpriteRenderComponent : public RenderComponent
{
    public:
        PlayerSpriteRenderComponent();

        void init(Scene& scene) override;
        void update(Scene& scene, const sf::Time& dt, sf::RenderTarget &target, RendererPseudo3D& renderer) override;

        ~PlayerSpriteRenderComponent();
    private:
        glm::vec3 calculateVelocity(glm::vec3 position, glm::vec3 target, float speed);
        SpriteAnimation playerSprite;

        sf::RectangleShape rect;
		bool isLookingLeft;

};
