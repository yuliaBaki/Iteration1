#include "TextureManager.h"
#include <iostream>

namespace RessourceManager{
    TextureManager::TextureManager()
    {
        //ctor
    }

    std::shared_ptr<sf::Texture> TextureManager::get(const std::string& name)
    {
        const auto i = p_textures.find(name);
        if(i != p_textures.end())
            return i->second;
        else{
            std::shared_ptr<sf::Texture> p_texture = std::make_shared<sf::Texture>();
            p_texture->loadFromFile(name);
            p_textures.insert({name, p_texture});
            return p_texture;
        }
    }

    void TextureManager::removeUnused()
    {
        for(auto i = p_textures.begin(); i != p_textures.end();)
            if(i->second.use_count() == 1)
                i = p_textures.erase(i);
            else
                ++i;
    }

    std::unordered_map<std::string, std::shared_ptr<sf::Texture>> TextureManager::p_textures;
}
