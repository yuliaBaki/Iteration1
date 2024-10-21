#include "PointPhysicComponent.h"

#include <iostream>
#include <thsan/gameObject/componentData/ComponentData.h>

PointPhysicComponent::PointPhysicComponent(glm::vec3 position)
	:position(position)
{
}

void PointPhysicComponent::init( Scene& scene)
{
	if (!parent->hasComponent(COMPONENT_DATA::TRANSFORM))
		parent->setComponent(COMPONENT_DATA::TRANSFORM, new Transform());

	Transform* transform = m_parent->getComponent<Transform>(COMPONENT_DATA::TRANSFORM);
	transform->position = position;
}

void PointPhysicComponent::setPosition(glm::vec3 position)
{
	this->position = position;
	Transform* transform = m_parent->getComponent<Transform>(COMPONENT_DATA::TRANSFORM);
	transform->position = position;
}

void PointPhysicComponent::update( Scene& scene, const sf::Time& dt)
{

}

PointPhysicComponent::~PointPhysicComponent()
{
	//dtor
}
