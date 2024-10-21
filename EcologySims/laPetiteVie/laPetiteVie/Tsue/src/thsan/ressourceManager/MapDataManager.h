//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     MapData data = nlohmann::json::parse(jsonString);

#pragma once

#include "../vendors/nlohmann/json.hpp"

#define VERSION    0.353

namespace data {
    using nlohmann::json;

    inline json get_untyped(const json& j, const char* property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json& j, std::string property) {
        return get_untyped(j, property.data());
    }


    struct Color {
        int64_t r;
        int64_t g;
        int64_t b;
        int64_t a;
    };

    struct Position {
        double x;
        double y;
        double z;
    };

    struct ImageStatic {
        std::string name;
        std::string path;
        std::string depth_path;
        std::string emission_path;
        Color key_color;
        Position position;
        double scale;
        double rotation;
        double depth_factor;
        bool flip_x;
        bool flip_y;
    };

    struct Seed {
        std::string type;
        int64_t x;
        int64_t y;
    };

    struct SpriteStatic {
        std::string name;
        std::string path;
        bool has_depth;
        bool has_emission;
        std::string current_animation;
        Color key_color;
        Position position;
        double scale;
        double rotation;
        double depth_factor;
        bool flip_x;
        bool flip_y;
    };

    struct NormalBlurr {
        double x;
        double y;
    };

    struct LightProperties {
        std::string name;

        bool isEnable;
        bool isDirection;
        bool isPoint;
        bool isSpot;
        bool isCursor;

        Position position;
        Position direction;

        double attenuationConstant;
        double attenuationLinear;
        double attenuationQuadratic;

        Color ambient;
        Color diffuse;
        Color specular;
    };


    struct voxelTerrain {
        std::string terrain_color_path;
        std::string terrain_height_path;
        std::string terrain_collision_height_path;
        std::string terrain_collision_mask_path;
        Position position;
        Position offset_camera_position;
        NormalBlurr normal_blurr;
        int64_t normal_blur_iteration;
        int64_t blur_type;
        int64_t FOV;
        double lod;
        double lod_start_at;
        double angle;
        double height;
        double horizon;
        double scale_height;
        double sprite_render_distance;
        double terrain_render_distance;
        double screen_width;
        double screen_height;
        double shininess;

        bool isViewTypeFixed;
    };

    struct MapData {
        voxelTerrain voxel_terrain;
        std::vector<ImageStatic> image_static;
        std::vector<SpriteStatic> sprite_static;
        std::vector<Seed> seed;
        std::vector<LightProperties> lights;
        double version;
    };

    void from_json(const json& j, data::Color& x);
    void to_json(json& j, const data::Color& x);

    void from_json(const json& j, data::Position& x);
    void to_json(json& j, const data::Position& x);

    void from_json(const json& j, data::ImageStatic& x);
    void to_json(json& j, const data::ImageStatic& x);

    void from_json(const json& j, data::Seed& x);
    void to_json(json& j, const data::Seed& x);

    void from_json(const json& j, data::SpriteStatic& x);
    void to_json(json& j, const data::SpriteStatic& x);

    void from_json(const json& j, data::NormalBlurr& x);
    void to_json(json& j, const data::NormalBlurr& x);

    void from_json(const json& j, data::LightProperties& x);
    void to_json(json& j, const data::LightProperties& x);

    void from_json(const json& j, data::voxelTerrain& x);
    void to_json(json& j, const data::voxelTerrain& x);

    void from_json(const json& j, data::MapData& x);
    void to_json(json& j, const data::MapData& x);

    inline void from_json(const json& j, data::Color& x) {
        x.r = j.at("r").get<int64_t>();
        x.g = j.at("g").get<int64_t>();
        x.b = j.at("b").get<int64_t>();
        x.a = j.at("a").get<int64_t>();
    }

    inline void to_json(json& j, const data::Color& x) {
        j = json::object();
        j["r"] = x.r;
        j["g"] = x.g;
        j["b"] = x.b;
        j["a"] = x.a;
    }

    inline void from_json(const json& j, data::Position& x) {
        x.x = j.at("x").get<double>();
        x.y = j.at("y").get<double>();
        x.z = j.at("z").get<double>();
    }

    inline void to_json(json& j, const data::Position& x) {
        j = json::object();
        j["x"] = x.x;
        j["y"] = x.y;
        j["z"] = x.z;
    }

    inline void from_json(const json& j, data::ImageStatic& x) {
        x.name = j.at("name").get<std::string>();
        x.path = j.at("path").get<std::string>();
        x.depth_path = j.at("depth_path").get<std::string>();
        x.emission_path = j.at("emission_path").get<std::string>();
        x.key_color = j.at("key_color").get<data::Color>();
        x.position = j.at("position").get<data::Position>();
        x.scale = j.at("scale").get<double>();
        x.rotation = j.at("rotation").get<double>();
        x.flip_x = j.at("flip_x").get<bool>();
        x.flip_y = j.at("flip_y").get<bool>();
        x.depth_factor = j.at("depth_factor").get<double>();
    }

    inline void to_json(json& j, const data::ImageStatic& x) {
        j = json::object();
        j["name"] = x.name;
        j["path"] = x.path;
        j["depth_path"] = x.depth_path;
        j["emission_path"] = x.emission_path;
        j["key_color"] = x.key_color;
        j["position"] = x.position;
        j["scale"] = x.scale;
        j["rotation"] = x.rotation;
        j["flip_x"] = x.flip_x;
        j["flip_y"] = x.flip_y;
        j["depth_factor"] = x.depth_factor;
    }

    inline void from_json(const json& j, data::Seed& x) {
        x.type = j.at("type").get<std::string>();
        x.x = j.at("x").get<int64_t>();
        x.y = j.at("y").get<int64_t>();
    }

    inline void to_json(json& j, const data::Seed& x) {
        j = json::object();
        j["type"] = x.type;
        j["x"] = x.x;
        j["y"] = x.y;
    }

    inline void from_json(const json& j, data::SpriteStatic& x) {
        x.name = j.at("name").get<std::string>();
        x.path = j.at("path").get<std::string>();
        x.has_depth = j.at("has_depth").get<bool>();
        x.has_emission = j.at("has_emission").get<bool>();
        x.current_animation = j.at("current_animation").get<std::string>();
        x.key_color = j.at("key_color").get<data::Color>();
        x.position = j.at("position").get<data::Position>();
        x.scale = j.at("scale").get<double>();
        x.rotation = j.at("rotation").get<double>();
        x.flip_x = j.at("flip_x").get<bool>();
        x.flip_y = j.at("flip_y").get<bool>();
        x.depth_factor = j.at("depth_factor").get<double>();
    }

    inline void to_json(json& j, const data::SpriteStatic& x) {
        j = json::object();
        j["name"] = x.name;
        j["path"] = x.path;
        j["has_depth"] = x.has_depth;
        j["has_emission"] = x.has_emission;
        j["current_animation"] = x.current_animation;
        j["key_color"] = x.key_color;
        j["position"] = x.position;
        j["scale"] = x.scale;
        j["rotation"] = x.rotation;
        j["flip_x"] = x.flip_x;
        j["flip_y"] = x.flip_y;
        j["depth_factor"] = x.depth_factor;

    }

    inline void from_json(const json& j, data::NormalBlurr& x) {
        x.x = j.at("x").get<double>();
        x.y = j.at("y").get<double>();
    }

    inline void to_json(json& j, const data::NormalBlurr& x) {
        j = json::object();
        j["x"] = x.x;
        j["y"] = x.y;
    }    
    
    inline void from_json(const json& j, data::LightProperties& x) {
        x.name = j.at("name").get<std::string>();
        x.isCursor = j.at("isCursor").get<bool>();
        x.isDirection = j.at("isDirection").get<bool>(); 
        x.isEnable = j.at("isEnable").get<bool>();
        x.isPoint = j.at("isPoint").get<bool>();
        x.isSpot = j.at("isSpot").get<bool>();

        x.ambient = j.at("ambient").get<data::Color>();
        x.diffuse = j.at("diffuse").get<data::Color>();
        x.specular = j.at("specular").get<data::Color>();

        x.attenuationConstant = j.at("attenuationConstant").get<double>();
        x.attenuationLinear = j.at("attenuationLinear").get<double>();
        x.attenuationQuadratic = j.at("attenuationQuadratic").get<double>();

        x.position = j.at("position").get<data::Position>();
        x.direction = j.at("direction").get<data::Position>();
    }

    inline void to_json(json& j, const data::LightProperties& x) {
        j = json::object();
        j["name"] = x.name;
        j["isCursor"] = x.isCursor;
        j["isDirection"] = x.isDirection;
        j["isEnable"] = x.isEnable;
        j["isPoint"] = x.isPoint;
        j["isSpot"] = x.isSpot;

        j["ambient"] = x.ambient;
        j["diffuse"] = x.diffuse;
        j["specular"] = x.specular;

        j["attenuationConstant"] = x.attenuationConstant;
        j["attenuationLinear"] = x.attenuationLinear;
        j["attenuationQuadratic"] = x.attenuationQuadratic;

        j["position"] = x.position;
        j["direction"] = x.direction;
    }
    
    inline void from_json(const json& j, data::voxelTerrain& x) {
        x.terrain_color_path = j.at("terrain_color_path").get<std::string>();
        x.terrain_height_path = j.at("terrain_height_path").get<std::string>();
        x.terrain_collision_height_path = j.at("terrain_collision_height_path").get<std::string>();
        x.terrain_collision_mask_path = j.at("terrain_collision_mask_path").get<std::string>();
        x.position = j.at("position").get<data::Position>();
        x.offset_camera_position = j.at("offsetCameraPosition").get<data::Position>();
        x.normal_blurr = j.at("normal_blurr").get<data::NormalBlurr>();
        x.normal_blur_iteration = j.at("normal_blur_iteration").get<int64_t>();
        x.lod = j.at("LOD").get<double>();
        x.lod_start_at = j.at("LOD_startAt").get<double>();
        x.angle = j.at("angle").get<double>();
        x.height = j.at("height").get<double>();
        x.horizon = j.at("horizon").get<double>();
        x.scale_height = j.at("scaleHeight").get<double>();
        x.sprite_render_distance = j.at("SpriteRenderDistance").get<double>();
        x.terrain_render_distance = j.at("terrainRenderDistance").get<double>();
        x.screen_width = j.at("screen_width").get<double>();
        x.screen_height = j.at("screen_height").get<double>();
        x.shininess = j.at("shininess").get<double>();
        x.FOV = j.at("FOV").get<int64_t>();
        x.isViewTypeFixed = j.at("isViewTypeFixed").get<bool>();
        x.blur_type = j.at("blur_type").get<int64_t>();
    }

    inline void to_json(json& j, const data::voxelTerrain& x) {
        j = json::object();
        j["terrain_color_path"] = x.terrain_color_path;
        j["terrain_height_path"] = x.terrain_height_path;
        j["terrain_collision_height_path"] = x.terrain_collision_height_path;
        j["terrain_collision_mask_path"] = x.terrain_collision_mask_path;
        j["position"] = x.position;
        j["offsetCameraPosition"] = x.offset_camera_position;
        j["normal_blurr"] = x.normal_blurr;
        j["normal_blur_iteration"] = x.normal_blur_iteration;
        j["LOD"] = x.lod;
        j["LOD_startAt"] = x.lod_start_at;
        j["angle"] = x.angle;
        j["height"] = x.height;
        j["horizon"] = x.horizon;
        j["scaleHeight"] = x.scale_height;
        j["SpriteRenderDistance"] = x.sprite_render_distance;
        j["terrainRenderDistance"] = x.terrain_render_distance;
        j["screen_width"] = x.screen_width;
        j["screen_height"] = x.screen_height;
        j["shininess"] = x.shininess;
        j["FOV"] = x.FOV;
        j["isViewTypeFixed"] = x.isViewTypeFixed;
        j["blur_type"] = x.blur_type;
    }

    inline void updateTo350(json& j) {
        j["version"] = 0.350;
    }

    inline void updateTo351(json& j, double &version) {
        if (version < 0.350)
            updateTo350(j);

        j["voxel_terrain"]["FOV"] = 1024;
        j["version"] = 0.351;
    }

    inline void updateTo352(json& j, double& version) {
        if (version <= 0.351)
            updateTo351(j, version);

        j["voxel_terrain"]["isViewTypeFixed"] = true;
        j["voxel_terrain"]["blur_type"] = 1;
        j["SpriteStatic"]["depth_factor"] = 1.0;
        j["ImageStatic"]["depth_factor"] = 1.0;
    }

    inline void updateTo353(json& j, double& version) {
        if (version <= 0.352)
            updateTo352(j, version);

        unsigned c = 0;
        for (auto &light : j["lights"]) {
            c++;
            light["name"] = "light" + c;
        }
        j["version"] = 0.353;
    }
    inline void from_json(const json& j, data::MapData& x) {

        json updated_j = j;

        x.voxel_terrain = updated_j.at("voxel_terrain").get<data::voxelTerrain>();
        x.image_static = updated_j.at("image_static").get<std::vector<data::ImageStatic>>();
        x.sprite_static = updated_j.at("sprite_static").get<std::vector<data::SpriteStatic>>();
        x.lights = updated_j.at("lights").get<std::vector<data::LightProperties>>();
        x.seed = updated_j.at("seed").get<std::vector<data::Seed>>();
        x.version = updated_j.at("version").get<double>();


        if (j.find("version") == j.end()) {
            updateTo353(updated_j, x.version);
        }
        else if (j.at("version").get<double>() < VERSION) {
            updateTo353(updated_j, x.version);
        }
    }

    inline void to_json(json& j, const data::MapData& x) {
        j = json::object();
        j["version"] = VERSION;
        j["voxel_terrain"] = x.voxel_terrain;
        j["image_static"] = x.image_static;
        j["sprite_static"] = x.sprite_static;
        j["lights"] = x.lights;
        j["seed"] = x.seed;
    }
}

class Scene;

namespace RessourceManager {
    class MapManager
    {
    public:
        MapManager() = delete;
        static std::shared_ptr<data::MapData> get(const std::string& path);
        static bool save(const std::string& path);
        static bool save(const std::string& path, std::shared_ptr<data::MapData> mapData);
        static void removeUnused();
    private:
        static std::unordered_map<std::string, std::shared_ptr<data::MapData>> p_mapDatas;
    };
}



