#include "RendererPseudo3D_preRender.h"
#include "../ressourceManager/TextureManager.h"
#include <iostream>

RendererPseudo3D_preRender::RendererPseudo3D_preRender(int width, int height,
    std::string terrainTexturePath,
    std::string depthTerrainTexturePath,
    std::string positionXTerrainTexturePath,
    std::string positionYTerrainTexturePath,
    std::string positionZTerrainTexturePath,
    std::string normalTerrainTexturePath) :
    RendererPseudo3D(width, height, VIEW_TYPE::PRE_RENDER_VIEW),
    was_rendered(false)
{
    terrainTexture = RessourceManager::TextureManager::get(terrainTexturePath);
    depthTerrainTexture = RessourceManager::TextureManager::get(depthTerrainTexturePath);
    positionXTerrainTexture = RessourceManager::TextureManager::get(positionXTerrainTexturePath);
    positionYTerrainTexture = RessourceManager::TextureManager::get(positionYTerrainTexturePath);
    positionZTerrainTexture = RessourceManager::TextureManager::get(positionZTerrainTexturePath);
    normalTerrainTexture = RessourceManager::TextureManager::get(normalTerrainTexturePath);

}

sf::Image RendererPseudo3D_preRender::getImage(EXPORT_IMAGE_TYPE export_image_type)
{
    if (export_image_type == EXPORT_IMAGE_TYPE::POSITION_X)
        return positionXTerrainTexture->copyToImage();
    else if (export_image_type == EXPORT_IMAGE_TYPE::POSITION_Y)
        return positionYTerrainTexture->copyToImage();
    else if (export_image_type == EXPORT_IMAGE_TYPE::POSITION_Z)
        return positionZTerrainTexture->copyToImage();
    else if (export_image_type == EXPORT_IMAGE_TYPE::DEPTH)
        return depthTerrainTexture->copyToImage();
    else if (export_image_type == EXPORT_IMAGE_TYPE::COLOR)
        return terrainTexture->copyToImage();
    else if (export_image_type == EXPORT_IMAGE_TYPE::NORMAL)
        return terrain->normal_map_texture.copyToImage();
    return sf::Image();
}

RendererPseudo3D_preRender::~RendererPseudo3D_preRender()
{
}

void RendererPseudo3D_preRender::renderTerrain()
{
    //presque rien, parce qu'on utilise les pre-rendu
    if (!has_voxel_terrain)
        return;

    float terrain_size = terrain->getSize();
    half_size = sf::Vector2f(terrain_size / 2, terrain_size / 2);
}

void RendererPseudo3D_preRender::setScreenResolution(float screen_width, float screen_height)
{
    // nothing because fixed terrain is immutable
}

void RendererPseudo3D_preRender::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!has_voxel_terrain)
        return;

    sf::Shader::bind(&*p_shaderTerrain);

    p_shaderTerrain->setUniform("is_debug_normal", false);
    p_shaderTerrain->setUniform("is_debug_position", false);
    p_shaderTerrain->setUniform("is_debug_normalized_scale", false);
    p_shaderTerrain->setUniform("is_debug_position_normalized_scale", false);
    p_shaderTerrain->setUniform("is_debug_depth", false);
    p_shaderTerrain->setUniform("is_debug_none", false);


    if (debug_rendering == Debug_rendering::debug_normal)
        p_shaderTerrain->setUniform("is_debug_normal", true);
    else if (debug_rendering == Debug_rendering::debug_position)
        p_shaderTerrain->setUniform("is_debug_position", true);
    else if (debug_rendering == Debug_rendering::debug_normalized_scale)
        p_shaderTerrain->setUniform("is_debug_normalized_scale", true);
    else if (debug_rendering == Debug_rendering::debug_position_normalized_scale)
        p_shaderTerrain->setUniform("is_debug_position_normalized_scale", true);
    else if (debug_rendering == Debug_rendering::debug_depth)
        p_shaderTerrain->setUniform("is_debug_depth", true);
    else
        p_shaderTerrain->setUniform("is_debug_none", true);


    p_shaderTerrain->setUniform("directionalLight_direction", directionalLight.direction);
    p_shaderTerrain->setUniform("directionalLight_ambient", sf::Glsl::Vec4(directionalLight.ambient));
    p_shaderTerrain->setUniform("directionalLight_diffuse", sf::Glsl::Vec4(directionalLight.diffuse));
    p_shaderTerrain->setUniform("directionalLight_specular", sf::Glsl::Vec4(directionalLight.specular));
    p_shaderTerrain->setUniform("fogColor", sf::Glsl::Vec4(directionalLight.ambient));
    p_shaderTerrain->setUniform("time", time);

    p_shaderTerrain->setUniform("pointLightCount", (int)pointLights.size());
    p_shaderTerrain->setUniform("angle", data.angle);

    for (int i = 0; i < pointLights.size(); i++) {
        LightProperties pointlight = pointLights[i];
        std::string si = std::to_string(i);
        p_shaderTerrain->setUniform("pointLights[" + si + "].position", pointlight.position);
        p_shaderTerrain->setUniform("pointLights[" + si + "].ambient", sf::Glsl::Vec4(pointlight.ambient));
        p_shaderTerrain->setUniform("pointLights[" + si + "].diffuse", sf::Glsl::Vec4(pointlight.diffuse));
        p_shaderTerrain->setUniform("pointLights[" + si + "].specular", sf::Glsl::Vec4(pointlight.specular));
        p_shaderTerrain->setUniform("pointLights[" + si + "].constant", pointlight.attenuationConstant);
        p_shaderTerrain->setUniform("pointLights[" + si + "].linear", pointlight.attenuationLinear);
        p_shaderTerrain->setUniform("pointLights[" + si + "].quadratic", pointlight.attenuationQuadratic);
    }

    p_shaderTerrain->setUniform("isDepth", false);
    p_shaderTerrain->setUniform("texture_depth", depthTexture.getTexture());
    p_shaderTerrain->setUniform("texture_height", *(terrain->texture_height));
    p_shaderTerrain->setUniform("camera_position", sf::Glsl::Vec3(data.position.x, data.position.y / data.scale_height, data.position.z));
    p_shaderTerrain->setUniform("far", data.terrain_render_distance);
    p_shaderTerrain->setUniform("width", static_cast<float>(data.screen_width));
    p_shaderTerrain->setUniform("height", static_cast<float>(data.screen_height));
    p_shaderTerrain->setUniform("normal_blur_width_factor", data.normal_blurr.x);
    p_shaderTerrain->setUniform("normal_blur_height_factor", data.normal_blurr.y);
    p_shaderTerrain->setUniform("normal_blur_iteration", static_cast<float>(data.normal_blur_iteration));
    p_shaderTerrain->setUniform("scale_height", data.scale_height);

    p_shaderTerrain->setUniform("isSprite", true);

    std::vector<SpriteData*> sprite_to_draw;
    for (SpriteData* s : spritesData)
        if (!s->spriteAnimation->isHidden()) {
            sprite_to_draw.push_back(s);
        }

    std::sort(sprite_to_draw.begin(), sprite_to_draw.end(), [](SpriteData* a, SpriteData* b) {
        return a->sprite_z > b->sprite_z;
        });

    for (SpriteData* s : sprite_to_draw) {
        SpriteAnimation* sa = s->spriteAnimation;

        float sprite_z = s->sprite_z;

        if (sprite_z < data.sprite_render_distance) {
            p_shaderTerrain->setUniform("texture_Yaxis", *positionYTerrainTexture);
            p_shaderTerrain->setUniform("texture", sf::Shader::CurrentTexture);

            p_shaderTerrain->setUniform("sprite_position", s->position);
            p_shaderTerrain->setUniform("sprite_texCoord_size", sf::Vector2f(sa->getCurrFrameRect().width, sa->getCurrFrameRect().height));
            p_shaderTerrain->setUniform("sprite_scale", s->scale);
            p_shaderTerrain->setUniform("sprite_rotation", s->rotation);
            p_shaderTerrain->setUniform("sprite_key_color", sf::Glsl::Vec4(sa->getKeyColor()));
            p_shaderTerrain->setUniform("shininess", static_cast<float>(sa->getShininess()));

            p_shaderTerrain->setUniform("sprite_texCoord_position", sf::Vector2f(sa->getCurrFrameRect().left, sa->getCurrFrameRect().top));
            p_shaderTerrain->setUniform("spriteSheet_size", sf::Vector2f(sa->getSpriteSheetSize().x, sa->getSpriteSheetSize().y));
            p_shaderTerrain->setUniform("NDC_sprite_position", sa->getPosition());
            p_shaderTerrain->setUniform("isFlipX", sa->isFlipX());
            p_shaderTerrain->setUniform("isFlipY", sa->isFlipY());
            p_shaderTerrain->setUniform("sprite_z", sprite_z);
            p_shaderTerrain->setUniform("sprite_depth_factor", sa->getDepthFactor());
            if (sa->hasDepthTexture())
                p_shaderTerrain->setUniform("sprite_depthTexture", *sa->GetDepthTexture());

            p_shaderTerrain->setUniform("has_sprite_depthTexture", sa->hasDepthTexture());

            if (sa->hasEmissionTexture())
                p_shaderTerrain->setUniform("sprite_emissionTexture", *sa->getEmissionTexture());
            p_shaderTerrain->setUniform("has_sprite_emissionTexture", sa->hasEmissionTexture());

            sf::Shader::bind(&*p_shaderTerrain);

            target.draw(*(sa));
        }
    }


    p_shaderTerrain->setUniform("texture", *terrainTexture);

    p_shaderTerrain->setUniform("isSprite", false);
    p_shaderTerrain->setUniform("isFixed", true);
    p_shaderTerrain->setUniform("isFixedPreRender", false);
    p_shaderTerrain->setUniform("isDepth", false);

    p_shaderTerrain->setUniform("texture_depth", depthTexture.getTexture());
    p_shaderTerrain->setUniform("texture_depthTerrain", *depthTerrainTexture);

    p_shaderTerrain->setUniform("texture_height", *(terrain->texture_height));
    p_shaderTerrain->setUniform("camera_position", sf::Glsl::Vec3(data.position.x, data.position.y / data.scale_height, data.position.z));
    p_shaderTerrain->setUniform("far", data.terrain_render_distance);
    p_shaderTerrain->setUniform("terrain_size", static_cast<float>(terrain->getSize()));
    p_shaderTerrain->setUniform("width", static_cast<float>(data.screen_width));
    p_shaderTerrain->setUniform("height", static_cast<float>(data.screen_height));
    p_shaderTerrain->setUniform("texture_Xaxis", *positionXTerrainTexture);
    p_shaderTerrain->setUniform("texture_Yaxis", *positionYTerrainTexture);
    p_shaderTerrain->setUniform("texture_Zaxis", *positionZTerrainTexture);
    p_shaderTerrain->setUniform("texture_normalmap", *normalTerrainTexture);
    p_shaderTerrain->setUniform("shininess", data.shininess);
    p_shaderTerrain->setUniform("blur_type", static_cast<int>(data.blur_type));

    states.transform *= getTransform();
    states.shader = &*p_shaderTerrain;

    sf::Sprite sp;
    sp.setTexture(*terrainTexture, &states);
    target.draw(sp);

    sf::Shader::bind(0);


    for (auto& pair : rectangles_no_perspective)
        target.draw(*pair.second, sf::RenderStates::Default);
}

