#include "VoxelTerrain.h"
#include <SFML/Graphics/Image.hpp>
#include <iostream>
#include <math.h>
#include <glm/glm.hpp>

#include "../ressourceManager/TextureManager.h"


void VoxelTerrain::init(const std::string& pathTextureColor, const std::string& pathTextureHeight, const std::string& pathTextureCollisionHeight, const std::string& pathTextureCollision, float max_elevation)
{

    texture_color = RessourceManager::TextureManager::get(pathTextureColor);
    texture_height = RessourceManager::TextureManager::get(pathTextureHeight);
    texture_collisionHeight = RessourceManager::TextureManager::get(pathTextureCollisionHeight);
    texture_collision = RessourceManager::TextureManager::get(pathTextureCollision);

    sf::Image imgColor = texture_color.get()->copyToImage();
    sf::Image imgHeight = texture_height.get()->copyToImage();
    sf::Image imgCollisionHeight = texture_collisionHeight.get()->copyToImage();
    sf::Image imgCollision = texture_collision.get()->copyToImage();

    size = imgColor.getSize().x;

    refresh_normal(max_elevation);
    this->max_elevation = max_elevation;

    height_map.resize(size * size, 0);
    color_map.resize(size * size, sf::Color());
    collisionHeight_map.resize(size * size, 0.f);
    collision_map.resize(size * size, 0.f);

    for(unsigned int i = 0; i < size; i++)
        for(unsigned int j = 0; j < size; j++){
            color_map[i + j * size] = imgColor.getPixel(i, j);
            height_map[i + j * size] = imgHeight.getPixel(i, j).r;
            collisionHeight_map[i + j * size] = imgCollisionHeight.getPixel(i, j).r;
            collision_map[i + j * size] = imgCollision.getPixel(i, j).r;
        }
}

glm::vec3 calculate_normal(sf::Image& imgHeight, size_t size_x, size_t size_y, int i, int j, float scale_height) {
    float hL = 0;
    float hR = 0;
    float hD = 0;
    float hU = 0;

    if (i > 0 && i < size_x - 1 && j > 0 && j < size_y - 1) {
        float offset = 1.0;
        hL = imgHeight.getPixel(i - offset, j).r / 255.0f;
        hR = imgHeight.getPixel(i + offset, j).r / 255.0f;
        hD = imgHeight.getPixel(i, j + offset).r / 255.0f;
        hU = imgHeight.getPixel(i, j - offset).r / 255.0f;
    }

    glm::vec3 nTerrain;
    nTerrain.x = (hL - hR);
    nTerrain.y = 2.0f / scale_height; // 2 * (1/scale_height) * (256/scaleHeight)
    nTerrain.z = (hU - hD);

    nTerrain = glm::normalize(nTerrain);

    return nTerrain;
}
void VoxelTerrain::refresh_normal(float scale_height)
{
    sf::Image imgHeight = texture_height.get()->copyToImage();

    size_t size_x = size;
    size_t size_y = size;

    std::vector<glm::vec3> first_pass_normal_map;
    first_pass_normal_map.resize(size_x * size_y, glm::vec3(0.0, 0.0, 0.0));

    normal_map_image.create(size_x, size_y);
    normal_map.resize(size_x * size_y, sf::Vector3f(0.0, 0.0, 0.0));

    for (unsigned int i = 0; i < size_x; i++){
        for (unsigned int j = 0; j < size_y; j++) {

            glm::vec3 nTerrain = calculate_normal(imgHeight, size_x, size_y, i, j, scale_height);
            first_pass_normal_map[i + j * size_y] = nTerrain;
            normal_map_image.setPixel(i, j, sf::Color((nTerrain.x+1)*128, nTerrain.y * 255, (nTerrain.z+1)*128));
        }
    }

    for (unsigned int i = 0; i < size_x; i++)
        for (unsigned int j = 0; j < size_y; j++) {

            glm::vec3 left = glm::vec3(0.f, 0.f, 0.f);
            glm::vec3 up = glm::vec3(0.f, 0.f, 0.f);
            glm::vec3 right = glm::vec3(0.f, 0.f, 0.f);
            glm::vec3 down = glm::vec3(0.f, 0.f, 0.f);

            if (i > 0 && i < size_x - 1 && j > 0 && j < size_y - 1) {
                left = first_pass_normal_map[(i - 1) + j * size_y];
                up = first_pass_normal_map[i + (j - 1) * size_y];
                right = first_pass_normal_map[(i + 1) + j * size_y];
                down = first_pass_normal_map[i + (j + 1) * size_y];
            }

            glm::vec3 temp_normal = glm::vec3(up + down + left + right) * 0.25f;

            glm::vec3 nTerrain = first_pass_normal_map[i + j * size_y];
            nTerrain += glm::normalize(temp_normal);
            glm::normalize(nTerrain);

            normal_map[i + j * size_y] = sf::Vector3f(nTerrain.x, nTerrain.y, nTerrain.z);
    }

    normal_map_texture.loadFromImage(normal_map_image);
}

float VoxelTerrain::getPhysicalHeight(sf::Vector2f position)
{
    position.x = int(position.x);
    position.y = int(position.y);
    return collisionHeight_map[static_cast<int>((position.x + size / 2) + (position.y + size / 2) * size)];
}

float VoxelTerrain::getCollisionMask(sf::Vector2f position)
{
    position.x = int(position.x);
    position.y = int(position.y);
    return collision_map[static_cast<int>((position.x + size / 2) + (position.y + size / 2) * size)];
}

sf::Vector3f VoxelTerrain::getNormal(sf::Vector2f position)
{
    position.x = int(position.x);
    position.y = int(position.y);
    return normal_map[static_cast<int>((position.x + size / 2) + (position.y + size / 2) * size)];
}

float VoxelTerrain::getSize()
{
    return size;
}

float VoxelTerrain::getRenderHeight(sf::Vector2f position)
{
	position.x = int(position.x);
	position.y = int(position.y);
	return height_map[static_cast<int>((position.x + size / 2) + (position.y + size / 2) * size)];
}

