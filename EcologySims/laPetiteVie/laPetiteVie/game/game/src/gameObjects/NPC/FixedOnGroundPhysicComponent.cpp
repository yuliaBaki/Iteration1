#include "FixedOnGroundPhysicComponent.h"

#include <iostream>
#include <thsan/gameObject/componentData/ComponentData.h>
#include <glm/glm.hpp>

FixedOnGroundPhysicComponent::FixedOnGroundPhysicComponent()
{
    //ctor
}

void FixedOnGroundPhysicComponent::init(Scene& scene)
{
    if (!parent->hasComponent(COMPONENT_DATA::HITBOX))
        parent->setComponent(COMPONENT_DATA::HITBOX, new HitBox());

    CharacterStatData* charState{ nullptr };
    if (!parent->hasComponent(COMPONENT_DATA::CHARACTER_STAT)) {
        charState = new CharacterStatData();
        parent->setComponent(COMPONENT_DATA::CHARACTER_STAT, charState);
    }
    else {
        charState = parent->getComponent<CharacterStatData>(COMPONENT_DATA::CHARACTER_STAT);
    }

    scene.addToGroups(parent, { "proud fighter" });

    hitbox = parent->getComponent<HitBox>(COMPONENT_DATA::HITBOX);
    hitbox->position = glm::vec3(0, 0, 0);
    hitbox->collision_object = Collision_object::real;

    charState->atk = 5.f;
    charState->def = 1.f;
    charState->hp = 2.f;
    charState->speed = 15.f;
}


void FixedOnGroundPhysicComponent::update(Scene& scene, const sf::Time& dt)
{
    Transform* temp = parent->getComponent<Transform>(COMPONENT_DATA::TRANSFORM);
	temp->position.y = scene.getPhysicalHeightOnPosition(sf::Vector2f(temp->position.x, temp->position.z));
}

FixedOnGroundPhysicComponent::~FixedOnGroundPhysicComponent()
{
    //dtor
}
