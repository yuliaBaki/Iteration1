#pragma once

#include <unordered_map>
#include <memory>
#include <SFML/Audio.hpp>

namespace RessourceManager {
    class SoundBuffferManager
    {
    public:
        SoundBuffferManager();
        static std::shared_ptr<sf::SoundBuffer> get(const std::string& name);
        static void removeUnused();
    private:
        static std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> p_soundBuffers;
    };
}