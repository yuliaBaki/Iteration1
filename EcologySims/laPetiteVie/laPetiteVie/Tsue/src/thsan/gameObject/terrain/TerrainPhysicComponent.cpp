#include "TerrainPhysicComponent.h"

TerrainPhysicComponent::TerrainPhysicComponent()
{
    //ctor
}

void TerrainPhysicComponent::init(Scene& scene)
{

}


void TerrainPhysicComponent::update(Scene& scene, const sf::Time& dt)
{
    Transform* temp = parent->getComponent<Transform>(COMPONENT_DATA::TRANSFORM);

}

TerrainPhysicComponent::~TerrainPhysicComponent()
{
    //dtor
}
