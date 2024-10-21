#include "SpriteAnimationRenderComponent.h"
#include "../../ressourceManager/ShaderManager.h"
#include "../../vendors/nlohmann/json.hpp"
#include "../../util/helper.h"

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

SpriteAnimationRenderComponent::SpriteAnimationRenderComponent(const std::string& spriteSheetMetaDataPath, bool hasEmissionMap, bool hasDepthMap):
    spriteSheetMetaDataPath(spriteSheetMetaDataPath),
    hasEmissionMap(hasEmissionMap),
    hasDepthMap(hasDepthMap)
{
    
}

void SpriteAnimationRenderComponent::setDepthMap(const std::string& path)
{
    spriteAnimation.setDepthTexture(path);
}

void SpriteAnimationRenderComponent::setEmissionMap(const std::string& path)
{
    spriteAnimation.setEmissionTexture(path);
}


void SpriteAnimationRenderComponent::init(Scene& scene)
{
    if (!parent->hasComponent(COMPONENT_DATA::EDITABLE_SPRITE_ANIMATION)) {
        editableSpriteAnimationData = new EditableSpriteAnimationData();
        parent->setComponent(COMPONENT_DATA::EDITABLE_SPRITE_ANIMATION, editableSpriteAnimationData);
    }
    else
        editableSpriteAnimationData = parent->getComponent<EditableSpriteAnimationData>(COMPONENT_DATA::EDITABLE_SPRITE_ANIMATION);
    
    nlohmann::ordered_json j;
    readJson(spriteSheetMetaDataPath, &j);

    std::string img_path = j["meta"]["image"];
    std::size_t last_slash = 0;

    auto found = img_path.find("\\");
    if (found != std::string::npos)
        last_slash = img_path.find_last_of("\\");

    img_path = img_path.substr(last_slash, img_path.size());

    fs::path parent_path = spriteSheetMetaDataPath;
    spriteAnimation.init(parent_path.parent_path().string() + "\\" + img_path);
    
    std::vector<json> frames;

    for (auto& [key, value] : j["frames"].items()) {
        frames.push_back(value);
    }
    
    for (auto& [key, value] : j["meta"]["frameTags"].items()) {
        std::string name = value["name"];
        int begin = value["from"];
        int end = value["to"];

        editableSpriteAnimationData->list_of_animations.push_back(name);
        spriteAnimation.setCurrAnimation(name);

        for (int i = begin; i <= end; i++) {
            auto& frame = frames[i]["frame"];
            auto& duration = frames[i]["duration"];
            std::cout << " x : " << frame["x"] << ", y: " << frame["y"] << ", w : " << frame["w"] << ",h : " << frame["h"] << std::endl;
            spriteAnimation.addFrame(name, sf::IntRect(frame["x"], frame["y"], frame["w"], frame["h"]), sf::milliseconds(duration));
        }
    }

    editableSpriteAnimationData->current_animation = spriteAnimation.getCurrAnimation();
    editableSpriteAnimationData->key_color = sf::Color(0, 0, 0, 0);
    spriteAnimation.setKeyColor(editableSpriteAnimationData->key_color);


    editableSpriteAnimationData->path = spriteSheetMetaDataPath;
    editableSpriteAnimationData->flipY = false;
    editableSpriteAnimationData->flipX = false;


    if (hasEmissionMap)
        spriteAnimation.setEmissionTexture(parent_path.parent_path().string() + "\\" + img_path.substr(0, img_path.size() - 4) + "emission.png");
    if (hasDepthMap)
        spriteAnimation.setDepthTexture(parent_path.parent_path().string() + "\\" + img_path.substr(0, img_path.size() - 4) + "depth.png");


    editableSpriteAnimationData->has_emission = spriteAnimation.hasEmissionTexture();
    editableSpriteAnimationData->has_depth = spriteAnimation.hasDepthTexture();
  
}

void SpriteAnimationRenderComponent::update(Scene& scene, const sf::Time& dt, sf::RenderTarget& target, RendererPseudo3D& rendererPseudo3D)
{
    if (spriteAnimation.getShininess() != editableSpriteAnimationData->shininess)
        spriteAnimation.setShininess(editableSpriteAnimationData->shininess);

    if (spriteAnimation.isFlipX() != editableSpriteAnimationData->flipX)
        spriteAnimation.flipX();

    if (spriteAnimation.isFlipY() != editableSpriteAnimationData->flipY)
        spriteAnimation.flipY();

    spriteAnimation.setKeyColor(editableSpriteAnimationData->key_color);

    if(spriteAnimation.getCurrAnimation() != editableSpriteAnimationData->current_animation)
        spriteAnimation.setCurrAnimation(editableSpriteAnimationData->current_animation);
    spriteAnimation.update(dt);

    Transform* transform = parent->getComponent<Transform>(COMPONENT_DATA::TRANSFORM);
    glm::vec3 position = transform->position;
    glm::vec2 scale = transform->scale;
    float rotation = transform->rotation;
    position.x = int(position.x + 0.5);
    position.z = int(position.z + 0.5);
    position.y += scene.getRenderHeightOnPosition(sf::Vector2f(position.x, position.z));

    spriteAnimation.setDepthFactor(editableSpriteAnimationData->depth_factor);

    rendererPseudo3D.add(&spriteAnimation, sf::Vector3f(position.x, position.y, position.z), sf::Vector2f(scale.x, scale.y), rotation);
}

 
void SpriteAnimationRenderComponent::setAnimation(const std::string& name)
{
    if(spriteAnimation.hasAnimation(name))
        spriteAnimation.setCurrAnimation(name);
}

SpriteAnimationRenderComponent::~SpriteAnimationRenderComponent()
{

}
