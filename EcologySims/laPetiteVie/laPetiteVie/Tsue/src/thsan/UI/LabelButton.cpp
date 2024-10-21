#include "LabelButton.h"
#include "../ressourceManager/FontManager.h"

UI::LabelButton::LabelButton(const std::string& name, const std::string& font_location, sf::Vector2f position, const std::string& text, float height, sf::Color fontColor)
	:Button(name, position, sf::Vector2f(0.f, 0.f), false)
{
	this->text = text;
	this->fontColor = fontColor;
	this->size.y = height;
	setFont(font_location);
	renderText.setString(text);
	renderText.setFont(*p_font);
	renderText.setCharacterSize(fontSize); // ??? fontSize never initialize
}

void UI::LabelButton::update(const sf::Time & deltaTime)
{
	elapsedTime += deltaTime;
	calculateDimenstion();

	renderText.setFillColor(fontColor);
	renderText.setCharacterSize(getGlobalSize().y);
	renderText.setPosition(getGlobalPosition());

	for (Widget* widget : childs)
		widget->update(deltaTime);
}

void UI::LabelButton::setFontColor(sf::Color color)
{
	fontColor = color;
}

sf::Color UI::LabelButton::getFontColor()
{
	return fontColor;
}

void UI::LabelButton::setFont(const std::string & font_location)
{
	p_font = RessourceManager::FontManager::get(font_location);
}

void UI::LabelButton::setFont(std::shared_ptr<sf::Font> p_font)
{
	this->p_font = p_font;
}

std::shared_ptr<sf::Font> UI::LabelButton::getFont()
{
	return p_font;
}

void UI::LabelButton::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (!hidden) {
		sf::RectangleShape rect;
		target.draw(renderText);

		for (Widget* widget : childs) {
			if (widget)
				target.draw(*widget);
		}
	}

}
