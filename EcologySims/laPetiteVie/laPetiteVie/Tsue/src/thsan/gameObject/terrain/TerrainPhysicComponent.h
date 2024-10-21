#pragma once

#include "../component/PhysicComponent.h"


class TerrainPhysicComponent : public PhysicComponent
{
    public:
        TerrainPhysicComponent();

        void init(Scene& scene) override;
        void update(Scene& scene, const sf::Time& dt) override;
        ~TerrainPhysicComponent();

};

