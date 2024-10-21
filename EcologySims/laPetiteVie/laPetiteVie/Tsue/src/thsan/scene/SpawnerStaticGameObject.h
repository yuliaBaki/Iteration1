#pragma once

#include "Scene.h"
#include "../ressourceManager/MapDataManager.h"
#include "../gameObject/GameObjectFactory.h"

// PassKey Pattern is used in this class
// https://chromium.googlesource.com/chromium/src/+/HEAD/docs/patterns/passkey.md

class GameObjectFactory;

class SpawnerStaticGameObject
{
public:
	class PassKey {
		private:
			PassKey() = default;
			~PassKey() = default;

		friend Scene;
	};
	SpawnerStaticGameObject(PassKey key, Scene* target);
	~SpawnerStaticGameObject() = default;

	void spawn(data::MapData mapData);
	void spawn(data::ImageStatic imageStatic);
	void spawn(data::SpriteStatic spriteStatic);
	void spawn(data::LightProperties light);
	void spawn(data::voxelTerrain voxelTerrain);

	void spawnMultiple(std::vector<data::ImageStatic> images);
	void spawnMultiple(std::vector<data::SpriteStatic> sprites);
	void spawnMultiple(std::vector<data::LightProperties> lights);

private:
	Scene* target;
	std::unique_ptr<GameObjectFactory> gameObjectFactory;
};

