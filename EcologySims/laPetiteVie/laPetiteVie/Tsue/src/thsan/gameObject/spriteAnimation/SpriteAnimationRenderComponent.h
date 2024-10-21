#pragma once

#include "../component/RenderComponent.h"
#include "../../graphic/SpriteAnimation.h"
#include "../../ressourceManager/TextureManager.h"

class SpriteAnimationRenderComponent : public RenderComponent
{
public:
    SpriteAnimationRenderComponent(const std::string& spriteSheetMetaDataPath, bool hasEmissionMap = false, bool hasDepthMap = false);

    void setDepthMap(const std::string& path);
    void setEmissionMap(const std::string& path);
    void init(Scene& scene) override;
    void update(Scene& scene, const sf::Time& dt, sf::RenderTarget& target, RendererPseudo3D& rendererPseudo3D) override;

    void setAnimation(const std::string& name);

    ~SpriteAnimationRenderComponent();
private:
    std::string spriteSheetMetaDataPath;
    bool hasEmissionMap;
    bool hasDepthMap;
    SpriteAnimation spriteAnimation;
    EditableSpriteAnimationData* editableSpriteAnimationData;
};