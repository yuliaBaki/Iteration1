#include "ShaderManager.h"

namespace RessourceManager{
    ShaderManager::ShaderManager()
    {
        //ctor
    }

    std::shared_ptr<sf::Shader> ShaderManager::get(const std::string& vertPath, const std::string& fragPath)
    {
        const auto i = p_shaders.find(vertPath+fragPath);
        if(i != p_shaders.end())
            return i->second;
        else{
            std::shared_ptr<sf::Shader> p_shader = std::make_shared<sf::Shader>();
            p_shader->loadFromFile(vertPath, fragPath);
            p_shaders.insert({vertPath+fragPath, p_shader});
            return p_shader;
        }
    }

    void ShaderManager::removeUnused()
    {
        for(auto i = p_shaders.begin(); i != p_shaders.end();)
            if(i->second.use_count() == 1)
                i = p_shaders.erase(i);
            else
                ++i;
    }

    std::unordered_map<std::string, std::shared_ptr<sf::Shader>> ShaderManager::p_shaders;
}
