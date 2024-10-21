#pragma once

#include "GameObject.h"
#include "../graphic/light/PointLight.h"

class GameObject;

class GameObjectFactory {

public:
	GameObjectFactory(Scene* target);
	~GameObjectFactory() = default;
	GameObject* createStaticImage(const std::string& name, const std::string& path, const std::string& emission_path = "", const std::string& depth_path = "");
	GameObject* createStaticSprite(const std::string& name, const std::string& spriteSheetMetaDataPath, bool hasEmissionMap = false, bool hasDepthMap = false);
	GameObject* createPointLight(const std::string& name, PointLight* pl);
	GameObject* createVolumetricPostProcess(const std::string& name, GameObject* targeted_go, const HitBox& hb, const VolumetricPostProcessData& vppd);
private:
	Scene* target;
};