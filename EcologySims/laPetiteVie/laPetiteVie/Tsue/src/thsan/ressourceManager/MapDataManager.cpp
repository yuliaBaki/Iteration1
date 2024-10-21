#include "MapDataManager.h"
#include "../util/helper.h"

namespace RessourceManager {

    std::shared_ptr<data::MapData> MapManager::get(const std::string& path)
    {
        const auto i = p_mapDatas.find(path);
        if (i != p_mapDatas.end())
            return i->second;
        else {
            std::shared_ptr<data::MapData> p_mapData = std::make_shared<data::MapData>();
            json j;
            readJson(path, &j);
            *p_mapData = nlohmann::json::parse(j.dump());
            p_mapDatas.insert({ path, p_mapData });
            return p_mapData;
        }

    }

    bool MapManager::save(const std::string& path)
    {
        bool success = false;
        const auto i = p_mapDatas.find(path);
        if (i != p_mapDatas.end()) {
            json j;
            data::to_json(j, *i->second);
            writeJson(path, j);
            success = true;
        }

        return success;
    }

    bool MapManager::save(const std::string& path, std::shared_ptr<data::MapData> mapData)
    {
        bool success = true;

        if (!mapData)
            success = false;
        else {
            json j;
            data::to_json(j, *mapData);
            writeJson(path, j);
        }

        return success;
    }

    void MapManager::removeUnused()
    {
        for (auto i = p_mapDatas.begin(); i != p_mapDatas.end();)
            if (i->second.use_count() == 1)
                i = p_mapDatas.erase(i);
            else
                ++i;
    }

    std::unordered_map<std::string, std::shared_ptr<data::MapData>> MapManager::p_mapDatas;

}