#pragma once
#include <string>
#include <sfml/Graphics/Text.hpp>
#include <sfml/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include "../vendors/RichText.h"
#include <unordered_map>
#include <map>
#include "Widget.h"


namespace UI {
	class LongText : public Widget
	{
	public:
		LongText(const std::string& name, const std::string& fontPath, sf::Vector2f position, std::string textPath, float fontHeight = 0.1f, float lineSpacing = 0.f);

		void stop();
		void resume();
		void enableAuto();
		void DisableAuto();
		bool isParagraphefinished();
		void clearText();
		bool isFinished();

		void setFontSize(float size);

		bool loadFromString(const std::string& text);
		bool loadFromFile(const std::string& path);

		virtual void update(const sf::Time& deltaTime) override;
		unsigned int getCurrClearParagrapheNumber();
		unsigned int totalClearParagrapheSize();

		void setFont(const std::string& font_location);
		void setFont(std::shared_ptr<sf::Font> p_font);
		std::shared_ptr<sf::Font> getFont();

	private:
		struct Paragraphe {
			Paragraphe() :clear(false) {}
			std::string text;
			sf::Time timePerChar;
			sf::Time waitTime;
			std::map < int, sf::Color > colors;
			bool clear;

		};

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		bool isStop;
		bool isAutomatic;
		bool done;
		
		sf::Time timer;
		sf::Time timerChar;
		std::vector<Paragraphe> paragraphes;
		sfe::RichText richText;
		float fontHeight;
		float lineSpacing;
		size_t currParagraphe;
		size_t currClearParagraphe;
		size_t totalClearParagraphe;
		std::shared_ptr<sf::Font> p_font;

		//update member variable
		Paragraphe* pp;
		bool isCharFinished{ false };
		int charCounter{ 0 };

	};
}
