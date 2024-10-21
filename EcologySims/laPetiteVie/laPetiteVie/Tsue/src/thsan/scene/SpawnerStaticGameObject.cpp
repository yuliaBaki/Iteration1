#include "SpawnerStaticGameObject.h"
#include "../gameObject/image/ImageRenderComponent.h"
#include "../gameObject/spriteAnimation/SpriteAnimationRenderComponent.h"

SpawnerStaticGameObject::SpawnerStaticGameObject(PassKey key, Scene* target):
    target(target)
{
    gameObjectFactory = std::make_unique<GameObjectFactory>(target);
}

void SpawnerStaticGameObject::spawn(data::MapData mapData)
{
    spawn(mapData.voxel_terrain);
    spawnMultiple(mapData.image_static);
    spawnMultiple(mapData.sprite_static);
    spawnMultiple(mapData.lights);
}

void SpawnerStaticGameObject::spawn(data::ImageStatic imageStatic)
{
    GameObject* go = gameObjectFactory->createStaticImage(imageStatic.name, imageStatic.path, imageStatic.emission_path, imageStatic.depth_path);

    EditableImageData* editable_image = go->getComponent<EditableImageData>(COMPONENT_DATA::EDITABLE_IMAGE);
    Transform* transform = go->getComponent<Transform>(COMPONENT_DATA::TRANSFORM);

    editable_image->flipX = imageStatic.flip_x;
    editable_image->flipY = imageStatic.flip_y;
    editable_image->depth_path = imageStatic.depth_path;
    editable_image->emission_path = imageStatic.emission_path;
    editable_image->key_color.r = imageStatic.key_color.r;
    editable_image->key_color.g = imageStatic.key_color.g;
    editable_image->key_color.b = imageStatic.key_color.b;
    editable_image->depth_factor = imageStatic.depth_factor;
    go->setName(imageStatic.name);
    editable_image->path = imageStatic.path;
    transform->position.x = imageStatic.position.x;
    transform->position.y = imageStatic.position.y;
    transform->position.z = imageStatic.position.z;
    transform->rotation = imageStatic.rotation;
    transform->scale.x = imageStatic.scale;
    transform->scale.y = imageStatic.scale;
}

void SpawnerStaticGameObject::spawn(data::SpriteStatic spriteStatic)
{
    //create go
    GameObject* go = gameObjectFactory->createStaticSprite(spriteStatic.name, spriteStatic.path, spriteStatic.has_emission, spriteStatic.has_depth);

    //enit le spriteAnimation Component
    EditableSpriteAnimationData* editable_sprite_animation_data = go->getComponent<EditableSpriteAnimationData>(COMPONENT_DATA::EDITABLE_SPRITE_ANIMATION);

    editable_sprite_animation_data->flipX = spriteStatic.flip_x;
    editable_sprite_animation_data->flipY = spriteStatic.flip_y;
    editable_sprite_animation_data->has_depth = spriteStatic.has_depth;
    editable_sprite_animation_data->has_emission = spriteStatic.has_emission;
    editable_sprite_animation_data->key_color.r = spriteStatic.key_color.r;
    editable_sprite_animation_data->key_color.g = spriteStatic.key_color.g;
    editable_sprite_animation_data->key_color.b = spriteStatic.key_color.b;
    editable_sprite_animation_data->path = spriteStatic.path;
    editable_sprite_animation_data->current_animation = spriteStatic.current_animation;
    editable_sprite_animation_data->depth_factor = spriteStatic.depth_factor;


    //init transform comp.
    Transform* transform = go->getComponent<Transform>(COMPONENT_DATA::TRANSFORM);

    transform->position.x = spriteStatic.position.x;
    transform->position.y = spriteStatic.position.y;
    transform->position.z = spriteStatic.position.z;
    transform->rotation = spriteStatic.rotation;
    transform->scale.x = spriteStatic.scale;
    transform->scale.y = spriteStatic.scale;
}

void SpawnerStaticGameObject::spawn(data::LightProperties light)
{
    if (light.isPoint) {
        PointLight* pl = target->createPointLight();
        pl->setAmbientColor(sf::Color(
            static_cast<sf::Uint8>(light.ambient.r),
            static_cast<sf::Uint8>(light.ambient.g),
            static_cast<sf::Uint8>(light.ambient.b)
        ));

        pl->setDiffuseColor(sf::Color(
            static_cast<sf::Uint8>(light.diffuse.r),
            static_cast<sf::Uint8>(light.diffuse.g),
            static_cast<sf::Uint8>(light.diffuse.b)
        ));

        pl->setSpecularColor(sf::Color(
            static_cast<sf::Uint8>(light.specular.r),
            static_cast<sf::Uint8>(light.specular.g),
            static_cast<sf::Uint8>(light.specular.b)
        ));

        pl->setPosition(sf::Vector3f(light.position.x, light.position.y, light.position.z));
        pl->enable(light.isEnable);

        pl->setAttenuation(light.attenuationConstant, light.attenuationLinear, light.attenuationQuadratic);
    }
    else if (light.isDirection) {
        target->setDirLight_ambient(sf::Color(
            static_cast<sf::Uint8>(light.ambient.r),
            static_cast<sf::Uint8>(light.ambient.g),
            static_cast<sf::Uint8>(light.ambient.b)
        ));

        target->setDirLight_diffuse(sf::Color(
            static_cast<sf::Uint8>(light.diffuse.r),
            static_cast<sf::Uint8>(light.diffuse.g),
            static_cast<sf::Uint8>(light.diffuse.b)
        ));


        target->setDirLight_specular(sf::Color(
            static_cast<sf::Uint8>(light.specular.r),
            static_cast<sf::Uint8>(light.specular.g),
            static_cast<sf::Uint8>(light.specular.b)
        ));

        target->setDirLight_direction(sf::Vector3f(light.direction.x, light.direction.y, light.direction.z));

        if (light.isEnable)
            target->enableDirectionalLight();
        else
            target->disableDirectionalLight();
    }
}

void SpawnerStaticGameObject::spawn(data::voxelTerrain voxelTerrain)
{
    VoxelTerrainData* vt = target->get_child("terrain")->getComponent<VoxelTerrainData>(COMPONENT_DATA::VOXEL_TERRAIN);

    vt->angle = voxelTerrain.angle;
    vt->height = voxelTerrain.height;
    vt->horizon = voxelTerrain.horizon;
    vt->LOD = voxelTerrain.lod;
    vt->LOD_start_at = voxelTerrain.lod_start_at;
    vt->normal_blurr.x = voxelTerrain.normal_blurr.x;
    vt->normal_blurr.y = voxelTerrain.normal_blurr.y;
    vt->normal_blur_iteration = voxelTerrain.normal_blur_iteration;
    vt->offset_camera_position.x = voxelTerrain.offset_camera_position.x;
    vt->offset_camera_position.y = voxelTerrain.offset_camera_position.y;
    vt->offset_camera_position.z = voxelTerrain.offset_camera_position.z;
    vt->position.x = voxelTerrain.position.x;
    vt->position.y = voxelTerrain.position.y;
    vt->position.z = voxelTerrain.position.z;
    vt->scale_height = voxelTerrain.scale_height;
    vt->screen_height = voxelTerrain.screen_height;
    vt->screen_width = voxelTerrain.screen_width;
    vt->sprite_render_distance = voxelTerrain.sprite_render_distance;
    vt->collision_height_map_path = voxelTerrain.terrain_collision_height_path;
    vt->collision_mask_map_path = voxelTerrain.terrain_collision_mask_path;
    vt->color_map_path = voxelTerrain.terrain_color_path;
    vt->height_map_path = voxelTerrain.terrain_height_path;
    vt->terrain_render_distance = voxelTerrain.terrain_render_distance;
    vt->shininess = voxelTerrain.shininess;
    vt->FOV = voxelTerrain.FOV;
    vt->blur_type = voxelTerrain.blur_type;
}

void SpawnerStaticGameObject::spawnMultiple(std::vector<data::ImageStatic> images)
{
    for (data::ImageStatic image_static : images)
        spawn(image_static);
}

void SpawnerStaticGameObject::spawnMultiple(std::vector<data::SpriteStatic> sprites)
{
    for (data::SpriteStatic sprite_static : sprites)
        spawn(sprite_static);
}

void SpawnerStaticGameObject::spawnMultiple(std::vector<data::LightProperties> lights)
{
    for (data::LightProperties light_data : lights)
        spawn(light_data);
}