#include "Label.h"
#include <algorithm>
#include <iostream>
#include "../ressourceManager/FontManager.h"

UI::Label::Label(const std::string& name, const std::string& font_location, sf::Vector2f position, const std::string& text, float height, sf::Color fontColor)
:Widget(name, position, sf::Vector2f(0.f,0.f), false)
{
	this->text = text;
	this->fontColor = fontColor;
	this->size.y = height;
	setFont(font_location);
	renderText.setString(text);
	renderText.setFont(*p_font);
	renderText.setCharacterSize(fontSize);
}

void UI::Label::update(const sf::Time& deltaTime)
{
	elapsedTime += deltaTime;
	calculateDimenstion();

	renderText.setFillColor(fontColor);
	renderText.setCharacterSize(getGlobalSize().y); //why?
	renderText.setPosition(getGlobalPosition());

	for (Widget* widget : childs)
		widget->update(deltaTime);


}

void UI::Label::setFontColor(sf::Color color)
{
	fontColor = color;
}

sf::Color UI::Label::getFontColor()
{
	return fontColor;
}

void UI::Label::setFont(const std::string& font_location)
{
	p_font = RessourceManager::FontManager::get(font_location);
}

void UI::Label::setFont(std::shared_ptr<sf::Font> p_font)
{
	this->p_font = p_font;
}

std::shared_ptr<sf::Font> UI::Label::getFont()
{
	return p_font;
}

void UI::Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!hidden) {
		target.draw(renderText);

		for (Widget* widget : childs) {
			if (widget)
				target.draw(*widget);
		}
	}
}
