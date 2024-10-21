#pragma once

#include <string>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Color.hpp>
#include <glm/glm.hpp>

#include <SFML/Graphics/Image.hpp>
#include <unordered_map>
#include "../../graphic/light/Light.h"
#include "../../graphic/PostProcess.h"
#include "../../util/helperMath.h"

enum class COMPONENT_DATA: unsigned int {
    NONE          = 0,
    TRANSFORM     = 1,
    UI            = 2,
    TILE_MAP      = 3,
    VOXEL_TERRAIN = 4,
	CAMERA		  = 5,
    HITBOX        = 6,
	PHYSIC_BODY   = 7,
    TREES         = 8,
    PLAYER        = 9,
    EDITABLE_SPRITE_ANIMATION = 10,
    EDITABLE_IMAGE = 11,
    LIGHT          = 12,
    VOLUME_POST_PROCESS = 13,
    BARS           = 14,
    CHARACTER_STAT = 15
};


struct ComponentData{};

struct Transform: public ComponentData{
    Transform(){
        this->position = glm::vec3(0.f, 0.f, 0.f);
        this->scale = glm::vec2(1.f, 1.f);
        this->rotation = 0.f;
        this->parent = nullptr;
    }

    glm::vec3 position;
    glm::vec2 scale;
    float rotation;

    Transform* parent;
};


// note: position is in the center of the box.
//      In fact, every single position in shape
//      in hitBox is intended that way.

struct Sphere {
    float radius;
    glm::vec3 position;
};

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
    glm::vec3 position;
};

struct Capsule {
    float radius;
    float height;
    glm::vec3 position;
};

enum class Collision_object {
    phantoms,
    real
};

struct HitBox: public ComponentData {
    std::vector<Sphere> spheres;
    std::vector<AABB> AABBs;
    std::vector<Capsule> capsules;
    glm::vec3 position{0.f, 0.f, 0.f};
    Collision_object collision_object{ Collision_object::phantoms};
};

struct UI_data: public ComponentData{
    UI_data(){
        isHover = false;
        isActive = false;
        isVisited = false;
        isLink = false;

        isText = false;
        isImage = false;
        isPanelAlert = true;
        isPanelDecoration = true;

        position = sf::Vector2f(0.f, 0.f);
        size = sf::Vector2f(100.f, 10.f);
        color = sf::Color(0, 0, 0, 255);

        text = "";
        path = "media/images/default.png";
    }

    bool isHover;
    bool isActive;
    bool isVisited;
    bool isLink;

    bool isText;
    bool isImage;
    bool isPanelAlert;
    bool isPanelDecoration;
	bool isPanelSelection;

    sf::Vector2f position;
    sf::Vector2f size;
	sf::Vector2f margin;
	sf::Vector2f padding;
    sf::Color color;

    std::string text;
    std::string path;

    std::vector<UI_data> childs;
};

struct TileMapData: public ComponentData{
    std::vector<uint8_t> tileRender;
    std::vector<uint8_t> tileCollision;
    std::vector<uint8_t> tileEvent;

    uint64_t tile_size_render;
    uint64_t tile_size_collision;
    uint64_t tile_size_event;

    uint64_t tile_grid_render;
    uint64_t tile_grid_collision;
    uint64_t tile_grid_event;
};

struct VoxelTerrainData: public ComponentData{
    std::string height_map_path;
    std::string color_map_path;
    std::string collision_height_map_path;
    std::string collision_mask_map_path;
    std::string event_map_path;

    sf::Vector3f offset_camera_position;
    sf::Vector3f position;
    sf::Vector2f normal_blurr;
    float screen_width;
    float screen_height;

    int normal_blur_iteration;
    float LOD;
    float LOD_start_at;
    float angle;
    float height;
    float horizon;
    float scale_height;
    float sprite_render_distance;
    float terrain_render_distance;
    float shininess;
    int FOV;
    int blur_type;
};

struct CameraData : public ComponentData {
    bool isPolar{false};
    std::string target_name{};
	glm::vec3 offset;
    glm::vec2 screen_position;
    sf::FloatRect soft_zone;
    sf::FloatRect dead_zone;
    sf::FloatRect on_screen_target_rect;
    float offset_distance;
    float angle;
    float Damping;
};

struct physicBodyData : public ComponentData {
	physicBodyData() {
		force = 0.f;
		friction = 0.99f;
		direction = glm::vec3(0.f, 0.f, 0.f);
        masse = 0.f;
	}
	float force;
	float friction;
    float masse;
    glm::vec3 acceleration;
    glm::vec3 velocity;
	glm::vec3 direction;

    bool isJump{false};
    bool isDoubleJump{false};
    bool onGround{true};

};

struct PlayerData : public ComponentData {
    enum class State {
        idle,
        walk,
        run,
        attack,
        forward_attack,
        plant,
        damage,
        death,
        jump,
        double_jump,
        fall,
        land,
        charge
    };

    bool isAiming{false};
    State curr_state;
    Transform* target_transform;
    std::string target_name;

    float atk;
    float def;
    float hp;
};

struct EditableSpriteAnimationData : public ComponentData {
    EditableSpriteAnimationData():
        key_color(sf::Color::Transparent),
        shininess(32),
        depth_factor(1.f)
    {
    }
    sf::Color key_color;
    std::vector<std::string> list_of_animations;
    std::string current_animation;
    std::string path;
    float depth_factor;
    int shininess;
    bool has_depth;
    bool has_emission;
    bool flipX;
    bool flipY;
    

};

struct EditableImageData : public ComponentData {
    EditableImageData():
        flipX(false),
        flipY(false),
        shininess(32),
        depth_factor(1.f){
    }
    bool flipX;
    bool flipY;
    int shininess;
    float depth_factor;
    std::string path;
    std::string depth_path;
    std::string emission_path;
    sf::Color key_color;
};

struct LightData : public ComponentData {
    LightData():
    isDecoration(false){
    }
    LightProperties lightProperties;
    bool isDecoration;
};

struct VolumetricPostProcessData : public ComponentData {
    Ease ease{ Ease::in };
    InterpolationType interpolation{ InterpolationType::linear };
    sf::Time in_interpolation_duration{ sf::seconds(1.0f) };
    sf::Time out_interpolation_duration{ sf::seconds(1.0f) };
    std::unordered_map<PostProcess::Effect, float> effects_and_opacity;
};

struct BarsData : public ComponentData {
    bool show_bar{ false };
    sf::Time duration{ sf::seconds(0.5f) };
    float end_speed{ 3.f }; // bars will hide at en_speed faster -> elapsed_time -= dt * end_speed
    float bar_screen_ratio_top{0.f};
    float bar_screen_ratio_bottom{0.f};
};

struct CharacterStatData : public ComponentData {

    struct Current {
        int hp;
        int mp;
    };

    Current curr;

    int hp;
    int mp;
    int atk;
    int def;
    int speed;
};
