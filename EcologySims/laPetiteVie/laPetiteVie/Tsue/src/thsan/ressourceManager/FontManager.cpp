#include "FontManager.h"

namespace RessourceManager {
	FontManager::FontManager()
	{
		//ctor
	}

	std::shared_ptr<sf::Font> FontManager::get(const std::string& name)
	{
		const auto i = p_fonts.find(name);
		if (i != p_fonts.end())
			return i->second;
		else {
			std::shared_ptr<sf::Font> p_font = std::make_shared<sf::Font>();
			p_font->loadFromFile(name);
			p_fonts.insert({ name, p_font });
			return p_font;
		}
	}

	void FontManager::removeUnused()
	{
		for (auto i = p_fonts.begin(); i != p_fonts.end();)
			if (i->second.use_count() == 1)
				i = p_fonts.erase(i);
			else
				++i;
	}


	std::unordered_map<std::string, std::shared_ptr<sf::Font>> FontManager::p_fonts;
}
