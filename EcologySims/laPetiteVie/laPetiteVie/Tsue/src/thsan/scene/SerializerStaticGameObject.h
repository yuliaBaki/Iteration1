#pragma once
#include "Scene.h"
#include "../ressourceManager/MapDataManager.h"
#include "../gameObject/componentData/ComponentData.h"

// PassKey Pattern is used in this class
// https://chromium.googlesource.com/chromium/src/+/HEAD/docs/patterns/passkey.md

class SerializerStaticGameObject
{
public:
	class PassKey {
	private:
		PassKey() = default;
		~PassKey() = default;

		friend Scene;
	};

	SerializerStaticGameObject(PassKey key, Scene* target);
	~SerializerStaticGameObject() = default;

	data::MapData serialize();
	data::voxelTerrain serialize(const VoxelTerrainData* voxelTerrainData);
	data::ImageStatic serialize(const EditableImageData* editableImageData, Transform* transform, const std::string& name);
	data::SpriteStatic serialize(const EditableSpriteAnimationData* editableSpriteAnimationData, Transform* transform, const std::string& name);
	data::LightProperties serialize(const LightData* lightData, const std::string& name);
	data::LightProperties serialize(const LightProperties* light, const std::string& name);

	//NO USE AS OF NOW, NO PURPOSE. to delete eventually if it keep no being made
	//std::vector<data::ImageStatic> serializeMultiple(std::vector<EditableImageData*> images);
	//std::vector<data::SpriteStatic> serializeMultiple(std::vector<EditableSpriteAnimationData*> sprites);
	//std::vector<data::LightProperties> serializeMultiple(std::vector<LightData*> lights);
private:
	Scene* target;
};
