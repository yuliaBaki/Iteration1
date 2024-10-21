#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <unordered_map>
#include <memory>
#include <SFML/Graphics.hpp>


namespace RessourceManager{
    class ShaderManager
    {
        public:
            ShaderManager();
            static std::shared_ptr<sf::Shader> get(const std::string& vertPath, const std::string& fragPath);
            static void removeUnused();
        private:
            static std::unordered_map<std::string, std::shared_ptr<sf::Shader>> p_shaders;
    };
}

#endif // SHADERMANAGER_H
