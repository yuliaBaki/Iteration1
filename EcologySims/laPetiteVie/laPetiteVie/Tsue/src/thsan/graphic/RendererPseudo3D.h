#ifndef RENDERERPSEUDO3D_H
#define RENDERERPSEUDO3D_H

#include "SpriteAnimation.h"
#include "VoxelTerrain.h"

#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Shader.hpp>

#include <memory>
#include <unordered_map>
#include <array>
#include <map>
#include "../gameObject/componentData/ComponentData.h"
#include "light/Light.h"

enum class VIEW_TYPE {
    FIXED_VIEW,
    DYNAMIC_VIEW,
    PRE_RENDER_VIEW
};

enum class Debug_rendering {
    none,
    debug_normal,
    debug_position,
    debug_normalized_scale,
    debug_position_normalized_scale,
    debug_depth
};

enum class BLUR_TYPE {
    NONE = 0,
    LOSANGE = 1,
    SQUARE = 2,
    TRIANGLE = 3,
    WALL = 4
};

enum class EXPORT_IMAGE_TYPE {
    POSITION_X = 0,
    POSITION_Y = 1,
    POSITION_Z = 2,
    DEPTH = 3,
    COLOR = 4,
    NORMAL = 5, 
    DEBUG_NORMAL = 6,
    DEBUG_POSITION = 7,
    DEBUG_DEPTH = 8
};


    class RendererPseudo3D : public sf::Drawable, public sf::Transformable
    {

    public:
        RendererPseudo3D(int width, int height, VIEW_TYPE view_type);

        void add(SpriteAnimation* spriteAnimation, sf::Vector3f pos, sf::Vector2f scale = sf::Vector2f(1.0, 1.0), float rotation = 0.f);
        void add(SpriteAnimation* spriteAnimation, glm::vec3 pos, glm::vec2 scale = glm::vec2(1.0, 1.0), float rotation = 0.f);

        void add(sf::CircleShape* circleShape, sf::Vector3f pos, sf::Vector2f scale = sf::Vector2f(1.0, 1.0), float rotation = 0.f);
        void add(sf::CircleShape* circleShape, glm::vec3 pos, glm::vec2 scale = glm::vec2(1.0, 1.0), float rotation = 0.f);

        void add(sf::RectangleShape* rect, sf::Vector3f pos, sf::Vector2f scale = sf::Vector2f(1.0, 1.0), float rotation = 0.f);
        void add(sf::RectangleShape* rectangleShape, glm::vec3 pos, glm::vec2 scale = glm::vec2(1.0, 1.0), float rotation = 0.f);

        void add_on_top_without_perspective(sf::RectangleShape* rectangleShape, int z = 0);

        void trackSpriteWithTag(SpriteAnimation* spriteAnimation, const std::string& tag);
        sf::FloatRect querySpriteRectAssociateWith(const std::string& tag);
        const std::vector<std::string>* getAllTags() const;

        void enableDebugRendering(Debug_rendering debug_rendering);
        void disableDebugRendring();

        void setTerrain(VoxelTerrain* terrain);

        void setParam(VoxelTerrainData data);
        VoxelTerrainData getParam();

        const sf::Texture& getDepthTexture();
        VIEW_TYPE getViewType();

        sf::Vector2f getHalfSize() { return half_size; };

        virtual sf::Image getImage(EXPORT_IMAGE_TYPE export_image_type) = 0;
        virtual ~RendererPseudo3D();

    protected:
        Debug_rendering debug_rendering;

        void render();
        virtual void renderTerrain() = 0;
        virtual void setScreenResolution(float screen_width, float screen_height) = 0;
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {};

        sf::Vector2f normalize_scale(sf::Vector2f scale);
        void clearSprites();

        struct SpriteData {
            SpriteData(SpriteAnimation* sa, sf::Vector3f position, sf::Vector2f scale, float rotation, float curr_height, float line_begin, float line_end, float dx, float sprite_z) :
                spriteAnimation(sa),
                position(position),
                scale(std::move(scale)),
                rotation(std::move(rotation)),
                curr_height(curr_height),
                line_begin(line_begin),
                line_end(line_end),
                dx(dx),
                sprite_z(sprite_z)
            {}
            SpriteAnimation* spriteAnimation;
            sf::Vector2f scale;
            sf::Vector3f position;
            float rotation;
            float curr_height;
            float line_begin;
            float line_end;
            float dx;
            float sprite_z;
        };

        VoxelTerrainData data;

        VIEW_TYPE view_type;
        VoxelTerrain* terrain;
        std::vector<float> ybuffer;
        sf::Vector2f half_size;

        std::vector<std::string> tags;
        std::unordered_map<std::string, SpriteAnimation*> spriteByTags;
        std::vector<SpriteData*> spritesData;
        std::vector<SpriteAnimation*> circleShape_spriteAnimations;
        std::vector<SpriteAnimation*> rectShape_spriteAnimations;
        std::vector<sf::Texture*> custom_textures;

        std::multimap<int, sf::RectangleShape*, std::less<int>> rectangles_no_perspective;

        sf::RenderTexture depthTexture;

        std::shared_ptr<sf::Shader> p_shaderTerrain;

        sf::VertexArray m_vertices;
        LightProperties directionalLight;
        std::vector<LightProperties> pointLights;
        float time;
        bool has_voxel_terrain;
        bool was_rendered{false};

        friend class Scene;
        friend class RendererPseudo3D_dynamic;
    };

#endif // RENDERERPSEUDO3D_H
