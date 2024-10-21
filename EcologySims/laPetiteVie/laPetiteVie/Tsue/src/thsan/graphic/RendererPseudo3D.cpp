#include "RendererPseudo3D.h"
#include <iostream>
#include <limits>

#include "../ressourceManager/ShaderManager.h"

RendererPseudo3D::RendererPseudo3D(int width, int height, VIEW_TYPE view_type) :
    has_voxel_terrain(false),
    view_type(view_type)
{
    debug_rendering = Debug_rendering::none;
    ybuffer = std::vector<float>(width, height);
    depthTexture.create(width, height);
    m_vertices.setPrimitiveType(sf::Lines);
    data.screen_width = width;
    data.screen_height = height;
    p_shaderTerrain = RessourceManager::ShaderManager::get("media/shader/terrain/terrain.vert", "media/shader/terrain/terrain.frag");
}

void RendererPseudo3D::add(SpriteAnimation* spriteAnimation, sf::Vector3f position, sf::Vector2f scale, float rotation)
{
    spriteAnimation->hide();

    scale = normalize_scale(scale);
    float size = terrain->getSize();
    position.x += size /2;
    position.z += size /2;

    sf::IntRect rect = spriteAnimation->getCurrFrameRect();
    float width = rect.width * scale.x;

    int i = static_cast<int>(position.x + position.z * size);
    SpriteData* sp = new SpriteData(spriteAnimation, position, scale, rotation, 0, position.x - width / 2, position.x + width / 2, 0, 0);
    spritesData.push_back(sp);

}

void RendererPseudo3D::add(SpriteAnimation* spriteAnimation, glm::vec3 pos, glm::vec2 scale, float rotation)
{
    add(spriteAnimation, sf::Vector3f(pos.x, pos.y, pos.z), sf::Vector2f(scale.x, scale.y), rotation);
}

void RendererPseudo3D::add(sf::CircleShape* circleShape, sf::Vector3f position, sf::Vector2f scale, float rotation)
{
    float size = terrain->getSize();

    scale = normalize_scale(scale);

    sf::RenderTexture* render_texture = new sf::RenderTexture();
    render_texture->create(circleShape->getLocalBounds().height, circleShape->getLocalBounds().height);
    render_texture->clear(sf::Color(0, 0, 0, 0));
    render_texture->draw(*circleShape);
    render_texture->display();

    sf::Texture* texture = new sf::Texture(render_texture->getTexture());
    custom_textures.push_back(texture);

    delete render_texture;

    SpriteAnimation* sa = new SpriteAnimation(*texture);
    circleShape_spriteAnimations.push_back(sa);

    sa->setKeyColor(sf::Color(0, 0, 0, 0));
    sa->setCurrAnimation("default");
    sa->addFrame("default", sf::IntRect(0, 0, circleShape->getLocalBounds().height, circleShape->getLocalBounds().height), sf::seconds(2.f));
    sa->update(sf::seconds(1.f));


    //ajout de sprite animation
    sa->hide();

    position.x += size / 2;
    position.z += size / 2;

    sf::IntRect rect = sa->getCurrFrameRect();
    float width = rect.width * scale.x;

    int i = static_cast<int>(position.x + position.z * size);
    SpriteData* sp = new SpriteData(sa, position, scale, rotation, 0, position.x - width / 2, position.x + width / 2, 0, 0);
    spritesData.push_back(sp);

}

void RendererPseudo3D::add(sf::CircleShape* circleShape, glm::vec3 pos, glm::vec2 scale, float rotation)
{
    add(circleShape, sf::Vector3f(pos.x, pos.y, pos.z), sf::Vector2f(scale.x, scale.y), rotation);
}

void RendererPseudo3D::add(sf::RectangleShape* rectShape, sf::Vector3f position, sf::Vector2f scale, float rotation)
{
    scale = normalize_scale(scale);

    sf::RenderTexture* render_texture = new sf::RenderTexture();
    render_texture->create(rectShape->getLocalBounds().width, rectShape->getLocalBounds().height);
    render_texture->clear(sf::Color(0.f, 0.f, 0.f, 0.f));
    render_texture->draw(*rectShape);
    render_texture->display();

    sf::Texture* texture = new sf::Texture(render_texture->getTexture());
    custom_textures.push_back(texture);

    delete render_texture;

    SpriteAnimation* sa = new SpriteAnimation(*texture);
    rectShape_spriteAnimations.push_back(sa);

    sa->setKeyColor(sf::Color(0, 0, 0, 0));
    sa->setCurrAnimation("default");
    sa->addFrame("default", sf::IntRect(0, 0, (int) rectShape->getLocalBounds().width, (int) rectShape->getLocalBounds().height), sf::seconds(2.f));
    sa->update(sf::seconds(1.f));


    //ajout de sprite animation
    sa->hide();

    float terrain_size = terrain->getSize();

    position.x += terrain_size / 2;
    position.z += terrain_size / 2;

    sf::IntRect rect = sa->getCurrFrameRect();
    float width = rect.width * scale.x;

    int i = static_cast<int>(position.x + position.z * terrain_size);
    SpriteData* sp = new SpriteData(sa, position, scale, rotation, 0, position.x - width / 2, position.x + width / 2, 0, 0);
    spritesData.push_back(sp);
}

void RendererPseudo3D::add(sf::RectangleShape* rectangleShape, glm::vec3 pos, glm::vec2 scale, float rotation)
{
    add(rectangleShape, sf::Vector3f(pos.x, pos.y, pos.z), sf::Vector2f(scale.x, scale.y), rotation);
}

void RendererPseudo3D::add_on_top_without_perspective(sf::RectangleShape* rectangleShape, int z)
{
    rectangles_no_perspective.insert({ z, rectangleShape });
}

void RendererPseudo3D::trackSpriteWithTag(SpriteAnimation* spriteAnimation, const std::string& tag)
{
    tags.push_back(tag);
    spriteByTags[tag] = spriteAnimation;
}

sf::FloatRect RendererPseudo3D::querySpriteRectAssociateWith(const std::string& tag)
{
    SpriteAnimation* sa = spriteByTags[tag];
    return  sa->getGlobalBounds();
}

const std::vector<std::string>* RendererPseudo3D::getAllTags() const
{
    return &tags;
}

VIEW_TYPE RendererPseudo3D::getViewType()
{
    return this->view_type;
}

void RendererPseudo3D::setTerrain(VoxelTerrain* terrain)
{
    has_voxel_terrain = true;
    this->terrain = terrain;
}


sf::Vector2f RendererPseudo3D::normalize_scale(sf::Vector2f scale)
{
    //l'engin peut seulement scale correctement quand les deux composantes sont égaux
    scale.y = scale.x;
    return scale;
}


void RendererPseudo3D::setParam(VoxelTerrainData data)
{
    if (this->data.screen_width != data.screen_width || this->data.screen_height != data.screen_height)
        setScreenResolution(data.screen_width, data.screen_height);
    this->data = data;
}

VoxelTerrainData RendererPseudo3D::getParam()
{
    return data;
}

void RendererPseudo3D::enableDebugRendering(Debug_rendering debug_rendering)
{
    this->debug_rendering = debug_rendering;
}

void RendererPseudo3D::clearSprites()
{
   spritesData.clear();

   for (SpriteAnimation* sa : circleShape_spriteAnimations)
       delete sa;

   circleShape_spriteAnimations.clear();


   for (SpriteAnimation* sa : rectShape_spriteAnimations)
       delete sa;

   rectShape_spriteAnimations.clear();

   for (sf::Texture* rt : custom_textures)
       delete rt;

   custom_textures.clear();

   spriteByTags.clear();
   tags.clear();
   rectangles_no_perspective.clear();
}

void RendererPseudo3D::disableDebugRendring()
{
    debug_rendering = Debug_rendering::none;
}

const sf::Texture& RendererPseudo3D::getDepthTexture()
{
    return depthTexture.getTexture();
}



void RendererPseudo3D::render()
{
    const double pi = 3.14159265358979323846;

    if (!has_voxel_terrain)
        return;

    float terrain_size = terrain->getSize();
    sf::Vector2f n = sf::Vector2f(cos(data.angle + pi / 2), sin(data.angle + pi / 2));

    //data.height = /*terrain->height_map[h_i + h_j * terrain->SIZE]*/ + data.height + data.cam_offset.y; //ligne commentaire qui force la camera au dessus de la carte

    if (!has_voxel_terrain)
        return;

    if (view_type == VIEW_TYPE::DYNAMIC_VIEW) {
        renderTerrain();
    }
    else if (!was_rendered) {
        renderTerrain();
        was_rendered = true;
    }


    for (SpriteData* spriteData : spritesData)
        if (spriteData->spriteAnimation->isHidden()) {

            spriteData->curr_height = spriteData->position.y;

            //X axis of device coordinate
            sf::Vector2f relative_to_camera = sf::Vector2f(
                (spriteData->position.x - half_size.x) - (data.position.x + data.offset_camera_position.x),
                (data.position.z + data.offset_camera_position.z) - (spriteData->position.z - half_size.y)
            );

            float position_dot_prod_with_direction = n.x * relative_to_camera.x + n.y * relative_to_camera.y;

            sf::Vector2f relative_to_direction = sf::Vector2f(position_dot_prod_with_direction * n.x, position_dot_prod_with_direction * n.y);

            sf::Vector2f x_axis_dist = relative_to_camera - relative_to_direction;
            sf::Vector2f distance = sf::Vector2f(
                sqrt(x_axis_dist.x * x_axis_dist.x + x_axis_dist.y * x_axis_dist.y),
                sqrt(relative_to_direction.x * relative_to_direction.x + relative_to_direction.y * relative_to_direction.y)
            );

            //vérification si la distance est à la droite du centre de l'ecran ou à la gauche.
            float dot = relative_to_camera.x * -n.y + relative_to_camera.y * n.x;
            if (dot > 0)
                distance.x *= -1;

            //vérification si la distance dans la profondeur est positif ou négatif, soit derrière la camera ou non
            dot = relative_to_camera.x * n.x + relative_to_camera.y * n.y;
            if (dot < 0)
                distance.y *= -1;

            float normalized_x_on_screen = (distance.x + distance.y) / (2 * distance.y);

            //Y axis of device coordinate and scale
            spriteData->sprite_z = distance.y;
            spriteData->dx = distance.y * 2 / data.FOV;
            float scale_final = spriteData->scale.x / spriteData->dx;
            sf::IntRect rect = spriteData->spriteAnimation->getCurrFrameRect();
            float sprite_height_on_screen = (data.position.y - spriteData->curr_height) / spriteData->sprite_z * data.scale_height + data.horizon;

            //sprite transform
            SpriteAnimation* sa = spriteData->spriteAnimation;

            if (distance.y > 0)
                sa->show();

            sa->setRotation(spriteData->rotation);
            sa->setScale(scale_final * sa->getScale().x, scale_final * sa->getScale().y);
            sa->setPosition(sf::Vector2f(normalized_x_on_screen * data.screen_width, sprite_height_on_screen + -(rect.height / 2) * scale_final));

        }

    depthTexture.clear();
    sf::RenderStates states;
    states.shader = &*p_shaderTerrain;

    p_shaderTerrain->setUniform("isSprite", true);
    p_shaderTerrain->setUniform("isDepth", true);
    p_shaderTerrain->setUniform("texture", sf::Shader::CurrentTexture);

    std::sort(spritesData.begin(), spritesData.end(), [](SpriteData* a, SpriteData* b) {
        return a->sprite_z > b->sprite_z;
        });

    int i = 0;
    for (SpriteData* s : spritesData)
        if (s->spriteAnimation)
            if (!s->spriteAnimation->isHidden()) {
                float sprite_z = s->sprite_z;

                if (sprite_z < data.sprite_render_distance) {
                    p_shaderTerrain->setUniform("sprite_position", s->position);
                    p_shaderTerrain->setUniform("sprite_key_color", sf::Glsl::Vec4(s->spriteAnimation->getKeyColor()));

                    depthTexture.draw(*(s->spriteAnimation), states);
                }
            }
    p_shaderTerrain->setUniform("isSprite", false);

    depthTexture.display();
}

RendererPseudo3D::~RendererPseudo3D()
{
}
