#pragma once


#include <thsan/gameObject/component/PhysicComponent.h>

class PointPhysicComponent : public PhysicComponent
{
public:
	PointPhysicComponent(glm::vec3 position = glm::vec3(0.f));

	void init(Scene& scene) override;
	void update( Scene& scene, const sf::Time& dt) override;
	void setPosition(glm::vec3 position);

	~PointPhysicComponent();
private:
	GameObject* m_parent;
	glm::vec3 position;

};

