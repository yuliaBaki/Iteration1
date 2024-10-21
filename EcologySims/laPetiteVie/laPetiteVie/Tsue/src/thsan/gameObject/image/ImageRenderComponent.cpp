#include "ImageRenderComponent.h"
#include "../../ressourceManager/ShaderManager.h"

#include <iostream>

ImageRenderComponent::ImageRenderComponent(const std::string& path, const std::string& emissionPath, const std::string& depthPath):
    image(path),
    path(path)
{
    if(emissionPath != "")
        image.setEmissionTexture(emissionPath);
    if(depthPath != "")
        image.setDepthTexture(depthPath);


}

void ImageRenderComponent::setDepthMap(const std::string& path)
{
    image.setDepthTexture(path);
    editableImageData->depth_path = image.getDepthTexturePath();
}

void ImageRenderComponent::setEmissionMap(const std::string& path)
{
    image.setEmissionTexture(path);
    editableImageData->emission_path = image.getEmissionTexturePath();
}

void ImageRenderComponent::init(Scene& scene)
{
    if (!parent->hasComponent(COMPONENT_DATA::EDITABLE_IMAGE)) {
        editableImageData = new EditableImageData();
        parent->setComponent(COMPONENT_DATA::EDITABLE_IMAGE, editableImageData);
    }
    else
        editableImageData = parent->getComponent<EditableImageData>(COMPONENT_DATA::EDITABLE_IMAGE);

    editableImageData->path = path;
    editableImageData->emission_path = image.getEmissionTexturePath();
    editableImageData->depth_path = image.getDepthTexturePath();

    image.setKeyColor(sf::Color(0, 0, 0, 0));
    image.setCurrAnimation("default");
    image.addFrame("default", sf::IntRect(0, 0, image.getSpriteSheetSize().x, image.getSpriteSheetSize().y), sf::seconds(0.1f));
}

void ImageRenderComponent::update( Scene& scene, const sf::Time& dt, sf::RenderTarget& target, RendererPseudo3D& rendererPseudo3D)
{
    if (image.getShininess() != editableImageData->shininess)
        image.setShininess(editableImageData->shininess);

    if (image.isFlipX() != editableImageData->flipX)
        image.flipX();

    if (image.isFlipY() != editableImageData->flipY)
        image.flipY();

    image.update(dt);

    Transform* transform = parent->getComponent<Transform>(COMPONENT_DATA::TRANSFORM);
    glm::vec3 position = transform->position;
    glm::vec2 scale = transform->scale;
    float rotation = transform->rotation;
    position.x = int(position.x + 0.5);
    position.z = int(position.z + 0.5);
    position.y += scene.getRenderHeightOnPosition(sf::Vector2f(position.x, position.z));

    image.setDepthFactor(editableImageData->depth_factor);


    rendererPseudo3D.add(&image, position, scale, rotation);

}

 
ImageRenderComponent::~ImageRenderComponent()
{

}
