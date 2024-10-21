#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <unordered_map>
#include <memory>
#include <SFML/Graphics/Font.hpp>


namespace RessourceManager {
	class FontManager
	{
	public:
		FontManager();
		static std::shared_ptr<sf::Font> get(const std::string& name);
		static void removeUnused();
	private:
		static std::unordered_map<std::string, std::shared_ptr<sf::Font>> p_fonts;
	};
}

#endif // FONTMANAGER_H
