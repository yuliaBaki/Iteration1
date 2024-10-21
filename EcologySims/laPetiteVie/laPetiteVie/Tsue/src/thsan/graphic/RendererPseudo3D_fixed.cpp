#include "RendererPseudo3D_fixed.h"
#include "../ressourceManager/ShaderManager.h"
#include <iostream>
RendererPseudo3D_fixed::RendererPseudo3D_fixed(int width, int height):
	RendererPseudo3D(width, height, VIEW_TYPE::FIXED_VIEW),
    was_rendered(false)
{
    p_shader_fixed_color    = RessourceManager::ShaderManager::get("media/shader/terrain/terrain_fixed_color.vert"   , "media/shader/terrain/terrain_fixed_color.frag"   );
    p_shader_fixed_depth    = RessourceManager::ShaderManager::get("media/shader/terrain/terrain_fixed_depth.vert"   , "media/shader/terrain/terrain_fixed_depth.frag"   );
    p_shader_fixed_position = RessourceManager::ShaderManager::get("media/shader/terrain/terrain_fixed_position.vert", "media/shader/terrain/terrain_fixed_position.frag");

    depthTerrainTexture.create(width, height);
    positionXTerrainTexture.create(width, height);
    positionYTerrainTexture.create(width, height);
    positionZTerrainTexture.create(width, height);
    terrainTexture.create(width, height);

}

sf::Image RendererPseudo3D_fixed::getImage(EXPORT_IMAGE_TYPE export_image_type)
{
    sf::Image image;
    if (export_image_type == EXPORT_IMAGE_TYPE::POSITION_X)
        return positionXTerrainTexture.getTexture().copyToImage();
    else if (export_image_type == EXPORT_IMAGE_TYPE::POSITION_Y)
        return positionYTerrainTexture.getTexture().copyToImage();
    else if (export_image_type == EXPORT_IMAGE_TYPE::POSITION_Z)
        return positionZTerrainTexture.getTexture().copyToImage();
    else if (export_image_type == EXPORT_IMAGE_TYPE::DEPTH)
        return depthTerrainTexture.getTexture().copyToImage();
    else if (export_image_type == EXPORT_IMAGE_TYPE::COLOR)
        return terrainTexture.getTexture().copyToImage();
    else if (export_image_type == EXPORT_IMAGE_TYPE::NORMAL)
        return terrain->normal_map_texture.copyToImage();
    else if (export_image_type == EXPORT_IMAGE_TYPE::NORMAL)
        return terrain->normal_map_texture.copyToImage();
    else if (export_image_type == EXPORT_IMAGE_TYPE::DEBUG_NORMAL ||
             export_image_type == EXPORT_IMAGE_TYPE::DEBUG_POSITION ||
             export_image_type == EXPORT_IMAGE_TYPE::DEBUG_DEPTH) {

        p_shaderTerrain->setUniform("texture", terrainTexture.getTexture());

        p_shaderTerrain->setUniform("isSprite", false);
        p_shaderTerrain->setUniform("isFixed", true);
        p_shaderTerrain->setUniform("isFixedPreRender", false);
        p_shaderTerrain->setUniform("isDepth", false);

        if(export_image_type == EXPORT_IMAGE_TYPE::DEBUG_NORMAL)
            p_shaderTerrain->setUniform("is_debug_normal", true);
        else if(export_image_type == EXPORT_IMAGE_TYPE::DEBUG_POSITION)
            p_shaderTerrain->setUniform("is_debug_position", true);
        else if (export_image_type == EXPORT_IMAGE_TYPE::DEBUG_DEPTH)
            p_shaderTerrain->setUniform("is_debug_depth", true);

        depthTexture.clear();
        p_shaderTerrain->setUniform("texture_depth", depthTexture.getTexture());
        p_shaderTerrain->setUniform("texture_depthTerrain", depthTerrainTexture.getTexture());

        /*

        RendererPseudo3D_data data;
        data.cam_offset = sf::Vector3f(0.0, 0.0, 0.0);
        data.distance = voxelMapData->terrainRenderDistance;
        data.height = voxelMapData->position.y;
        data.horizon = voxelMapData->horizon;
        data.LOD = voxelMapData->LOD;
        data.LOD_start_at = voxelMapData->LOD_startAt;
        data.normal_blur = voxelMapData->normal_blurr;
        data.normal_blur_iteration = voxelMapData->normal_blur_iteration;
        data.spriteRenderDistMax = voxelMapData->SpriteRenderDistance;

        if (rendererPseudo3D.getParam().scale_height != voxelMapData->scaleHeight) {
            m_voxelTerrain.refresh_normal(voxelMapData->scaleHeight);
        }

        data.scale_height = voxelMapData->scaleHeight;
        data.angle = voxelMapData->angle;
        data.position = sf::Vector2f(voxelMapData->position.x, voxelMapData->position.z);
        data.screen_width = voxelMapData->screen_width;
        data.screen_height = voxelMapData->screen_height;
        data.shininess = voxelMapData->shininess;
        data.FOV = voxelMapData->FOV;
        data.blur_type = static_cast<BLUR_TYPE>(voxelMapData->blur_type);

        */

        p_shaderTerrain->setUniform("texture_height", *(terrain->texture_height));
        p_shaderTerrain->setUniform("camera_position", sf::Glsl::Vec3(data.position.x, data.position.y / data.scale_height, data.position.z));
        p_shaderTerrain->setUniform("far", data.terrain_render_distance);
        p_shaderTerrain->setUniform("terrain_size", static_cast<float>(terrain->getSize()));
        p_shaderTerrain->setUniform("width", static_cast<float>(data.screen_width));
        p_shaderTerrain->setUniform("height", static_cast<float>(data.screen_height));
        p_shaderTerrain->setUniform("texture_Xaxis", positionXTerrainTexture.getTexture());
        p_shaderTerrain->setUniform("texture_Yaxis", positionYTerrainTexture.getTexture());
        p_shaderTerrain->setUniform("texture_Zaxis", positionZTerrainTexture.getTexture());
        p_shaderTerrain->setUniform("texture_normalmap", terrain->normal_map_texture);
        p_shaderTerrain->setUniform("shininess", data.shininess);
        p_shaderTerrain->setUniform("blur_type", static_cast<int>(data.blur_type));
        p_shaderTerrain->setUniform("normal_blur_iteration", static_cast<float>(data.normal_blur_iteration));


        sf::RenderStates states;
        states.transform *= getTransform();
        states.shader = &*p_shaderTerrain;
        sf::Sprite sp;
        sp.setTexture(terrainTexture.getTexture());
        sf::RenderTexture texture;
        texture.create(data.screen_width, data.screen_height);
        texture.clear();
        texture.draw(sp, states);
        texture.display();
        return texture.getTexture().copyToImage();
    }
}

RendererPseudo3D_fixed::~RendererPseudo3D_fixed()
{
}


void RendererPseudo3D_fixed::renderTerrain()
{
    if (!has_voxel_terrain)
        return;

    float terrain_size = terrain->getSize();
    half_size = sf::Vector2f(terrain_size / 2, terrain_size / 2);
    ybuffer = std::vector<float>(data.screen_width, data.screen_height);

    //REALLY NEED TO CHANGE THIS
    /////////////////////////////////////////
    float cz = 1.0f;
    float cdz = 1.0f;
    size_t counter = 0;
    while (cz < data.terrain_render_distance) {
        cz += cdz;
        if (cz > data.LOD_start_at)
            cdz += data.LOD;
        counter++;
    }

    m_vertices.resize(counter * data.screen_width * 2);
    /////////////////////////////////////////

    int total_row_distance = counter - 1;

    float z = 1.0f;
    float dz = 1.0f;
    int c = 0;
    int r = 0;

    sf::Vector2f position = sf::Vector2f(data.position.x, data.position.z);

    while (z < data.terrain_render_distance) {

        float sinphi = sin(data.angle);
        float cosphi = cos(data.angle);

        sf::Vector2f pleft = sf::Vector2f(-cosphi * z - sinphi * z,  sinphi * z - cosphi * z) + position + half_size;
        sf::Vector2f pright = sf::Vector2f(cosphi * z - sinphi * z, -sinphi * z - cosphi * z) + position + half_size;

        float dx = (pright.x - pleft.x) / data.FOV;
        float dy = (pright.y - pleft.y) / data.FOV;

        for (int i = 0; i < data.screen_width; i++) {

            int k = i * 2;
            int l = k + 1;

            int indiceK = k + (total_row_distance - c) * 2 * data.screen_width;
            int indiceL = l + (total_row_distance - c) * 2 * data.screen_width;

            int pixel_indice = static_cast<int>(pleft.x) + static_cast<int>(pleft.y) * terrain_size;

            if (pixel_indice >= 0 && pixel_indice < terrain_size * terrain_size) {

                float curr_height = terrain->height_map[pixel_indice];
                float height_on_screen = (data.position.y - curr_height) / z * data.scale_height + data.horizon;

                sf::Color curr_color = terrain->color_map[pixel_indice];
                //curr_color.a = z;

                //terrain check up
                if (pleft.x > 0 && pleft.y > 0 && pleft.x < terrain_size && pleft.y < terrain_size) {

                    if (ybuffer[i] - height_on_screen > 0) {

                        sf::Vertex* vertex_k = &m_vertices[indiceK];
                        sf::Vertex* vertex_l = &m_vertices[indiceL];

                        vertex_k->position = sf::Vector2f(i, height_on_screen);
                        vertex_l->position = sf::Vector2f(i, height_on_screen + (ybuffer[i] - height_on_screen));

                        //sf::Color curr_color = terrain->color_map[pixel_indice];
                        //m_vertices[indiceK].color = sf::Color( (z*255)/distance, (z*255)/distance, (z*255)/distance );
                        //m_vertices[indiceL].color = sf::Color( (z*255)/distance, (z*255)/distance, (z*255)/distance );;


                        vertex_k->color = curr_color;
                        vertex_l->color = curr_color;

                        sf::Vector2f texCoords(pleft.x / terrain_size, pleft.y / terrain_size);
                        vertex_k->texCoords = texCoords;
                        vertex_l->texCoords = texCoords;

                        if (height_on_screen <= ybuffer[i])
                            ybuffer[i] = height_on_screen;

                        r++;
                    }
                    else {
                        sf::Vector2f zero(0.f, 0.f);
                        m_vertices[indiceK].position = zero;
                        m_vertices[indiceL].position = zero;
                    }

                }
                else {
                    sf::Vector2f zero(0, 0);
                    m_vertices[indiceK].position = zero;
                    m_vertices[indiceL].position = zero;
                }
            }
            else {
                sf::Vector2f zero(0, 0);
                m_vertices[indiceK].position = zero;
                m_vertices[indiceL].position = zero;
            }

            pleft.x += dx;
            pleft.y += dy;
        }

        z += dz;
        if (z > data.LOD_start_at)
            dz += data.LOD;
        c++;
    }

    sf::RenderStates states;

    //terrain RenderTexture
    states.transform *= getTransform();

    terrainTexture.clear(sf::Color(0, 0, 0, 0));
    terrainTexture.draw(m_vertices, states);
    terrainTexture.display();

    //depth terrain RenderTexture 
    states.shader = &*p_shader_fixed_depth;

    p_shader_fixed_depth->setUniform("far", data.terrain_render_distance);
    p_shader_fixed_depth->setUniform("angle", data.angle);
    p_shader_fixed_depth->setUniform("terrain_size", static_cast<float>(terrain->getSize()));
    p_shader_fixed_depth->setUniform("camera_position", sf::Glsl::Vec3(data.position.x, data.position.y / data.scale_height, data.position.z));

    depthTerrainTexture.clear(sf::Color(255, 255, 255, 255));
    depthTerrainTexture.draw(m_vertices, states);
    depthTerrainTexture.display();

    //position X RenderTexture
    states.shader = &*p_shader_fixed_position;

    p_shader_fixed_position->setUniform("isXaxis", true);
    p_shader_fixed_position->setUniform("isYaxis", false);
    p_shader_fixed_position->setUniform("isZaxis", false);
    p_shader_fixed_position->setUniform("texture_height", *(terrain->texture_height));
    p_shader_fixed_position->setUniform("texture", (terrain->normal_map_texture));

    positionXTerrainTexture.clear(sf::Color(255, 255, 255, 255));
    positionXTerrainTexture.draw(m_vertices, states);
    positionXTerrainTexture.display();


    //position Y RenderTexture

    p_shader_fixed_position->setUniform("isXaxis", false);
    p_shader_fixed_position->setUniform("isYaxis", true);
    p_shader_fixed_position->setUniform("isZaxis", false);
    p_shader_fixed_position->setUniform("texture_height", *(terrain->texture_height));

    positionYTerrainTexture.clear(sf::Color(255, 255, 255, 255));
    positionYTerrainTexture.draw(m_vertices, states);
    positionYTerrainTexture.display();

    //position Z RenderTexture

    p_shader_fixed_position->setUniform("isXaxis", false);
    p_shader_fixed_position->setUniform("isYaxis", false);
    p_shader_fixed_position->setUniform("isZaxis", true);
    p_shader_fixed_position->setUniform("texture_height", *(terrain->texture_height));

    positionZTerrainTexture.clear(sf::Color(255, 255, 255, 255));
    positionZTerrainTexture.draw(m_vertices, states);
    positionZTerrainTexture.display();

}

void RendererPseudo3D_fixed::setScreenResolution(float screen_width, float screen_height)
{
    // nothing because fixed terrain is immutable
}

void RendererPseudo3D_fixed::draw(sf::RenderTarget& target, sf::RenderStates states) const
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
    p_shaderTerrain->setUniform("blur_type", static_cast<int>(data.blur_type));

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
            p_shaderTerrain->setUniform("texture_Yaxis", positionYTerrainTexture.getTexture());
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

    p_shaderTerrain->setUniform("texture", terrainTexture.getTexture());

    p_shaderTerrain->setUniform("isSprite", false);
    p_shaderTerrain->setUniform("isFixed", true);
    p_shaderTerrain->setUniform("isFixedPreRender", false);
    p_shaderTerrain->setUniform("isDepth", false);

    p_shaderTerrain->setUniform("texture_depth", depthTexture.getTexture());
    p_shaderTerrain->setUniform("texture_depthTerrain", depthTerrainTexture.getTexture());

    p_shaderTerrain->setUniform("texture_height", *(terrain->texture_height));
    p_shaderTerrain->setUniform("camera_position", sf::Glsl::Vec3(data.position.x, data.position.y / data.scale_height, data.position.z));
    p_shaderTerrain->setUniform("far", data.terrain_render_distance);
    p_shaderTerrain->setUniform("terrain_size", static_cast<float>(terrain->getSize()));
    p_shaderTerrain->setUniform("width", static_cast<float>(data.screen_width));
    p_shaderTerrain->setUniform("height", static_cast<float>(data.screen_height));
    p_shaderTerrain->setUniform("texture_Xaxis", positionXTerrainTexture.getTexture());
    p_shaderTerrain->setUniform("texture_Yaxis", positionYTerrainTexture.getTexture());
    p_shaderTerrain->setUniform("texture_Zaxis", positionZTerrainTexture.getTexture());
    p_shaderTerrain->setUniform("texture_normalmap", terrain->normal_map_texture);
    p_shaderTerrain->setUniform("shininess", data.shininess);
    p_shaderTerrain->setUniform("blur_type", static_cast<int>(data.blur_type));
    p_shaderTerrain->setUniform("normal_blur_iteration", static_cast<float>(data.normal_blur_iteration));



    states.transform *= getTransform();
    sf::Sprite sp;
    sp.setTexture(terrainTexture.getTexture());
    target.draw(sp);
    
    sf::Shader::bind(0);

    for (auto& pair : rectangles_no_perspective)
        target.draw(*pair.second, sf::RenderStates::Default);
}

