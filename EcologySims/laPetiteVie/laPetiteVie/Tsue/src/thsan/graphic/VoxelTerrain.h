#ifndef VOXELTERRAIN_H
#define VOXELTERRAIN_H

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector3.hpp>
#include <memory>


class  VoxelTerrain
{
    public:
        VoxelTerrain() = default;
        ~VoxelTerrain() = default;

        void init(const std::string &pathTextureColor, const std::string &pathTextureHeight, const std::string& pathTextureCollisionHeight = "", const std::string& pathTextureCollision = "" , float max_elevation = 1.0f);
        void refresh_normal(float scale_height);
		float getRenderHeight(sf::Vector2f position);
		float getPhysicalHeight(sf::Vector2f position);
		float getCollisionMask(sf::Vector2f position);
        sf::Vector3f getNormal(sf::Vector2f position);

        float getSize();

    private:

        std::size_t size;

        float max_elevation;

        std::vector<sf::Color> color_map;
        std::vector<float> height_map;
        std::vector<float> collisionHeight_map;
        std::vector<float> collision_map;
        std::vector<sf::Vector3f> normal_map;
        
        sf::Image normal_map_image;
        sf::Texture normal_map_texture;
        std::shared_ptr<sf::Texture> texture_height;
        std::shared_ptr<sf::Texture> texture_collisionHeight;
        std::shared_ptr<sf::Texture> texture_collision;
        std::shared_ptr<sf::Texture> texture_color;

        friend class Scene;
        friend class RendererPseudo3D;
        friend class RendererPseudo3D_fixed;
        friend class RendererPseudo3D_dynamic;
        friend class RendererPseudo3D_preRender;

};

#endif // VOXELTERRAIN_H
