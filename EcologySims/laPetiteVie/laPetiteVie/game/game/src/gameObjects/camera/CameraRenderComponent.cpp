#include "CameraRenderComponent.h"

#include <iostream>
#include "../../../../../dependencies/imGuIZMO.quat-master/imGuIZMO.quat/vgMath.h"

CameraRenderComponent::CameraRenderComponent()
{

}

void CameraRenderComponent::init(Scene& scene)
{
	cam_data = parent->getComponent<CameraData>(COMPONENT_DATA::CAMERA);
}

void CameraRenderComponent::update(Scene& scene, const sf::Time& dt, sf::RenderTarget& target, RendererPseudo3D& rendererPseudo3D)
{
	static auto renderer_tags = scene.getAllTags();
	renderer_tags = scene.getAllTags();
	PlayerData* player_data = scene.get_child("player")->getComponent<PlayerData>(COMPONENT_DATA::PLAYER);
	glm::vec3 player_position = scene.get_child("player")->getComponent<Transform>(COMPONENT_DATA::TRANSFORM)->position;
	float middle = scene.getRenderResolution().y / 2.f;



}


CameraRenderComponent::~CameraRenderComponent()
{

}
