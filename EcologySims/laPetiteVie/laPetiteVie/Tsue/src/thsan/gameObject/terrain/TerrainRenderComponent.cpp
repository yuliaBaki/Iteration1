#include "TerrainRenderComponent.h"

#include <iostream>
#include <SFML/Graphics/Glsl.hpp>

#include "../../ressourceManager/ShaderManager.h"
#include <glm/glm.hpp>

TerrainRenderComponent::TerrainRenderComponent(std::string height_map_path, std::string color_map_path, std::string collisionHeight_map_path, std::string collisionMask_map_path):
	height_map_path(height_map_path),
	color_map_path(color_map_path),
	collisionHeight_map_path(collisionHeight_map_path),
	collisionMask_map_path(collisionMask_map_path)
{

}

void TerrainRenderComponent::init(Scene& scene)
{
    if(!parent->hasComponent(COMPONENT_DATA::VOXEL_TERRAIN))
        parent->setComponent(COMPONENT_DATA::VOXEL_TERRAIN, new VoxelTerrainData());


    VoxelTerrainData* voxelMapData = parent->getComponent<VoxelTerrainData>(COMPONENT_DATA::VOXEL_TERRAIN);

    voxelMapData->color_map_path = color_map_path;
    voxelMapData->height_map_path = height_map_path;
    voxelMapData->collision_height_map_path = collisionHeight_map_path;
    voxelMapData->collision_mask_map_path = collisionMask_map_path;

	voxelMapData->screen_width = 1024;
	voxelMapData->screen_height = 768;

	voxelMapData->LOD = 2.0;
	voxelMapData->LOD_start_at = 200;
	voxelMapData->angle = 0;
	voxelMapData->height = 20;
	voxelMapData->horizon = 300;
	voxelMapData->sprite_render_distance = 100;
	voxelMapData->terrain_render_distance = 100;
	voxelMapData->scale_height = 256;
	voxelMapData->shininess = 32;
	voxelMapData->FOV = 1024;
	voxelMapData->blur_type = 1;

	m_voxelTerrain.init(voxelMapData->color_map_path, voxelMapData->height_map_path, voxelMapData->collision_height_map_path, voxelMapData->collision_mask_map_path, voxelMapData->scale_height);

    scene.enablePseudo3DRendering();
    scene.setTerrain(&m_voxelTerrain);
}

void TerrainRenderComponent::start(Scene& scene, RendererPseudo3D& rendererPseudo3D)
{
}

void TerrainRenderComponent::update(Scene& scene, const sf::Time& dt, sf::RenderTarget& target, RendererPseudo3D& rendererPseudo3D)
{

	static float time = 0;
	time += dt.asSeconds();

	VoxelTerrainData* voxelTerrainData = parent->getComponent<VoxelTerrainData>(COMPONENT_DATA::VOXEL_TERRAIN);

	if (rendererPseudo3D.getParam().scale_height != voxelTerrainData->scale_height) {
		m_voxelTerrain.refresh_normal(voxelTerrainData->scale_height);
	}

	rendererPseudo3D.setParam(*voxelTerrainData);
}


TerrainRenderComponent::~TerrainRenderComponent()
{

}