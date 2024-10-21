#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <unordered_map>
#include <memory>
#include <SFML/Graphics.hpp>


namespace RessourceManager{
    class TextureManager
    {
        public:
            TextureManager();
            static std::shared_ptr<sf::Texture> get(const std::string& name);
            static void removeUnused();
        private:
            static std::unordered_map<std::string, std::shared_ptr<sf::Texture>> p_textures;
    };
}

#endif // TEXTUREMANAGER_H
