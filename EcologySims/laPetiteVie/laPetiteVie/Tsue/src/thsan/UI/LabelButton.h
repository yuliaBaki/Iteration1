#pragma once

#include "Label.h"
#include "Button.h"


namespace UI {
	class LabelButton: public Button
	{
		public:
			LabelButton(const std::string& name, const std::string& font_location, sf::Vector2f position = sf::Vector2f(0.f, 0.f), const std::string& text = "", float height = 1.0, sf::Color fontColor = sf::Color::White);
			~LabelButton() = default;

			void update(const sf::Time& deltaTime) override;

			void setFontColor(sf::Color color);
			sf::Color getFontColor();

			void setFont(const std::string& font_location);
			void setFont(std::shared_ptr<sf::Font> p_font);
			std::shared_ptr<sf::Font> getFont();

		private:
			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

			std::string text;
			float fontSize;
			sf::Color fontColor{sf::Color::White};
			sf::Text renderText;
			std::shared_ptr<sf::Font> p_font;

	};
}

