#include "RendererPseudo3D_dynamic.h"
#include "RendererPseudo3D_fixed.h"
#include <iostream>
RendererPseudo3D_dynamic::RendererPseudo3D_dynamic(int width, int height):
	RendererPseudo3D(width, height, VIEW_TYPE::DYNAMIC_VIEW)
{

}

sf::Image RendererPseudo3D_dynamic::getImage(EXPORT_IMAGE_TYPE export_image_type)
{
    if (export_image_type == EXPORT_IMAGE_TYPE::DEBUG_NORMAL ||
        export_image_type == EXPORT_IMAGE_TYPE::DEBUG_POSITION ||
        export_image_type == EXPORT_IMAGE_TYPE::DEBUG_DEPTH) {

        if (export_image_type == EXPORT_IMAGE_TYPE::DEBUG_NORMAL)
            p_shaderTerrain->setUniform("is_debug_normal", true);
        else if (export_image_type == EXPORT_IMAGE_TYPE::DEBUG_POSITION)
            p_shaderTerrain->setUniform("is_debug_position", true);
        else if (export_image_type == EXPORT_IMAGE_TYPE::DEBUG_DEPTH)
            p_shaderTerrain->setUniform("is_debug_depth", true);


        p_shaderTerrain->setUniform("isSprite", false);
        p_shaderTerrain->setUniform("isFixedPreRender", false);
        p_shaderTerrain->setUniform("isFixed", false);
        p_shaderTerrain->setUniform("isDepth", false);
        p_shaderTerrain->setUniform("isXaxis", false);
        p_shaderTerrain->setUniform("isYaxis", false);
        p_shaderTerrain->setUniform("isZaxis", false);

        depthTexture.clear();
        p_shaderTerrain->setUniform("texture_depth", depthTexture.getTexture());

        p_shaderTerrain->setUniform("texture_height", *(terrain->texture_height));
        p_shaderTerrain->setUniform("camera_position", sf::Glsl::Vec3(data.position.x, data.position.y / data.scale_height, data.position.z));
        p_shaderTerrain->setUniform("far", data.terrain_render_distance);
        p_shaderTerrain->setUniform("terrain_size", static_cast<float>(terrain->getSize()));
        p_shaderTerrain->setUniform("width", static_cast<float>(data.screen_width));
        p_shaderTerrain->setUniform("height", static_cast<float>(data.screen_height));
        p_shaderTerrain->setUniform("shininess", data.shininess);
        p_shaderTerrain->setUniform("blur_type", static_cast<int>(data.blur_type));

        sf::RenderStates states;
        states.transform *= getTransform();
        states.texture = &(terrain->normal_map_texture);
        states.shader = &*p_shaderTerrain;

        sf::RenderTexture texture;
        texture.create(data.screen_width, data.screen_height);
        texture.clear();
        texture.draw(m_vertices, states);
        texture.display();
        return texture.getTexture().copyToImage();
    }
        else {
        std::unique_ptr<RendererPseudo3D> temp_fixed_renderer = std::make_unique<RendererPseudo3D_fixed>(data.screen_width, data.screen_height);
        temp_fixed_renderer->setTerrain(terrain);
        temp_fixed_renderer->setParam(data);
        temp_fixed_renderer->renderTerrain();
        temp_fixed_renderer->render();

        return temp_fixed_renderer->getImage(export_image_type);
    }

}


void RendererPseudo3D_dynamic::renderTerrain()
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
    sf::Vertex* vertex = &m_vertices[0];

    while (z < data.terrain_render_distance) {

        float sinphi = sin(data.angle);
        float cosphi = cos(data.angle);

        sf::Vector2f pleft = sf::Vector2f(-cosphi * z - sinphi * z, sinphi * z - cosphi * z) + position + half_size;
        sf::Vector2f pright = sf::Vector2f(cosphi * z - sinphi * z, -sinphi * z - cosphi * z) + position + half_size;

        float dx = (pright.x - pleft.x) / data.FOV;
        float dy = (pright.y - pleft.y) / data.FOV;

        for (int i = 0; i < data.screen_width; i++) {

            int k = i * 2;

            int indice = k + c * 2 * data.screen_width;
            sf::Vertex* curr_vertex = (vertex + indice);

            if (pleft.x > 0 && pleft.y > 0 && pleft.x < terrain_size && pleft.y < terrain_size) {


                int pixel_indice = static_cast<int>(pleft.x) + static_cast<int>(pleft.y) * terrain_size;

                float curr_height = terrain->height_map[pixel_indice];
                float height_on_screen = (data.position.y - curr_height) / z * data.scale_height + data.horizon;

                sf::Color curr_color = terrain->color_map[pixel_indice];
                //curr_color.a = z;

                //terrain check up
                //if (pleft.x > 0 && pleft.y > 0 && pleft.x < terrain_size && pleft.y < terrain_size) {

                    if (ybuffer[i] - height_on_screen > 0.f) {

                        curr_vertex[0].position = sf::Vector2f(i, height_on_screen);
                        curr_vertex[1].position = sf::Vector2f(i, height_on_screen + (ybuffer[i] - height_on_screen));

                        //sf::Color curr_color = terrain->color_map[pixel_indice];
                        //m_vertices[indiceK].color = sf::Color( (z*255)/distance, (z*255)/distance, (z*255)/distance );
                        //m_vertices[indiceL].color = sf::Color( (z*255)/distance, (z*255)/distance, (z*255)/distance );;


                        curr_vertex[0].color = curr_color;
                        curr_vertex[1].color = curr_color;

                        sf::Vector2f texCoords(pleft.x / terrain_size, pleft.y / terrain_size);
                        curr_vertex[0].texCoords = texCoords;
                        curr_vertex[1].texCoords = texCoords;

                        if (height_on_screen <= ybuffer[i])
                            ybuffer[i] = height_on_screen;

                        r++;
                    }
                    else {
                        sf::Vector2f zero(0.f, 0.f);

                        curr_vertex[0].position = zero;
                        curr_vertex[1].position = zero;
                    }

                //}
                /*
                else {
                    sf::Vector2f zero(0, 0);
                    m_vertices[indiceK].position = zero;
                    m_vertices[indiceL].position = zero;
                }
                */
            }
            else {
                sf::Vector2f zero(0, 0);
                
                curr_vertex[0].position = zero;
                curr_vertex[1].position = zero;
            }

            pleft.x += dx;
            pleft.y += dy;
        }

        z += dz;
        if (z > data.LOD_start_at)
            dz += data.LOD;
        c++;
    }
}

void RendererPseudo3D_dynamic::setScreenResolution(float screen_width, float screen_height)
{
    data.screen_width = screen_width;
    data.screen_height = screen_height;

    ybuffer = std::vector<float>(screen_width, screen_height);
    depthTexture.create(screen_width, screen_height);

    renderTerrain();
}

void RendererPseudo3D_dynamic::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!has_voxel_terrain)
        return;

    states.shader = &*p_shaderTerrain;

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


            target.draw(*(sa), states);
        }
    }

    states.shader = &*p_shaderTerrain;

    p_shaderTerrain->setUniform("isSprite", false);
    p_shaderTerrain->setUniform("isFixedPreRender", false);
    p_shaderTerrain->setUniform("isFixed", false);
    p_shaderTerrain->setUniform("isDepth", false);
    p_shaderTerrain->setUniform("isXaxis", false);
    p_shaderTerrain->setUniform("isYaxis", false);
    p_shaderTerrain->setUniform("isZaxis", false);

    p_shaderTerrain->setUniform("texture_normalmap", terrain->normal_map_texture);
    p_shaderTerrain->setUniform("texture_depth", depthTexture.getTexture());

    p_shaderTerrain->setUniform("texture_height", *(terrain->texture_height));
    p_shaderTerrain->setUniform("camera_position", sf::Glsl::Vec3(data.position.x, data.position.y / data.scale_height, data.position.z));
    p_shaderTerrain->setUniform("far", data.terrain_render_distance);
    p_shaderTerrain->setUniform("terrain_size", static_cast<float>(terrain->getSize()));
    p_shaderTerrain->setUniform("width", static_cast<float>(data.screen_width));
    p_shaderTerrain->setUniform("height", static_cast<float>(data.screen_height));
    p_shaderTerrain->setUniform("shininess", data.shininess);
    p_shaderTerrain->setUniform("blur_type", static_cast<int>(data.blur_type));
    p_shaderTerrain->setUniform("normal_blur_iteration", static_cast<float>(data.normal_blur_iteration));

    target.draw(m_vertices, states);

    for (auto& pair : rectangles_no_perspective)
        target.draw(*pair.second, sf::RenderStates::Default);
}
