#include "SoundBuffferManager.h"

namespace RessourceManager {
    SoundBuffferManager::SoundBuffferManager()
    {
        //ctor
    }

    std::shared_ptr<sf::SoundBuffer> SoundBuffferManager::get(const std::string& name)
    {
        const auto i = p_soundBuffers.find(name);
        if (i != p_soundBuffers.end())
            return i->second;
        else {
            std::shared_ptr<sf::SoundBuffer> p_texture = std::make_shared<sf::SoundBuffer>();
            p_texture->loadFromFile(name);
            p_soundBuffers.insert({ name, p_texture });
            return p_texture;
        }
    }

    void SoundBuffferManager::removeUnused()
    {
        for (auto i = p_soundBuffers.begin(); i != p_soundBuffers.end();)
            if (i->second.use_count() == 1)
                i = p_soundBuffers.erase(i);
            else
                ++i;
    }

    std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> SoundBuffferManager::p_soundBuffers;
}
