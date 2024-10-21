#include "LongText.h"
#include "../ressourceManager/FontManager.h"
#include <sfml/Graphics/RenderTarget.hpp>
#include <sfml/Graphics/CircleShape.hpp>
#include "../utils.h"




UI::LongText::LongText(const std::string& name, const std::string& fontPath, sf::Vector2f position, std::string textPath, float height, float lineSpacing)
	:Widget(name, position, sf::Vector2f(0.f, height), false),
	fontHeight(height),
	lineSpacing(lineSpacing),
	timer(sf::seconds(0.f)),
	timerChar(sf::seconds(0.f)),
	done(false),
	currClearParagraphe(0),
	currParagraphe(0),
	totalClearParagraphe(0),
	pp{nullptr}
{
	loadFromFile(textPath);

	setFont(fontPath);
	richText.setFont(*p_font);

}

void UI::LongText::stop()
{
	isStop = true;
}

void UI::LongText::resume()
{
	isStop = false;
}

void UI::LongText::enableAuto()
{
	isAutomatic = true;
}

void UI::LongText::DisableAuto()
{
	isAutomatic = false;
}

bool UI::LongText::isParagraphefinished()
{
	return isStop;
}

void UI::LongText::clearText()
{
	richText.clear();
}

bool UI::LongText::isFinished()
{
	if (currClearParagraphe < totalClearParagraphe)
		return false;
	else
		return true;
}

void UI::LongText::setFontSize(float size)
{
	this->size.y = size;
}

bool UI::LongText::loadFromString(const std::string& text)
{

	Paragraphe p;

	std::istringstream iss(text);
	std::vector<std::string> words{ std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{} };

	bool firstParagraphe = true;
	bool noSpace = false;
	

	for (const std::string& word : words)
	{
		if (word == "%p") {
			if (firstParagraphe)
				firstParagraphe = false;
			else {
				paragraphes.push_back(p);
				p = Paragraphe();
			}
		}

		else if (word.substr(0, 12) == "timerPerChar") {
			int beginParam = word.find_first_of("(") + 1;
			int endParam = word.find_first_of(")") - beginParam;
			std::string stringParams = word.substr(beginParam, endParam);
			std::vector<float> params = Utils::convertParamsToVector<float>(stringParams);
			p.timePerChar = sf::seconds(params[0]);
		}

		else if (word.substr(0, 5) == "pause") {
			int beginParam = word.find_first_of("(") + 1;
			int endParam = word.find_first_of(")") - beginParam;
			std::string stringParams = word.substr(beginParam, endParam);
			std::vector<float> params = Utils::convertParamsToVector<float>(stringParams);
			p.waitTime = sf::seconds(params[0]);
		}

		else if (word.substr(0, 5) == "clear") {
			p.clear = true;
			totalClearParagraphe++;
		}

		else if (word.at(0) == '\\') {
			if (word.substr(0, 6) == "\\color") {
				int beginParam = word.find_first_of("(") + 1;
				int endParam = word.find_first_of(")") - beginParam;
				std::string stringParams = word.substr(beginParam, endParam);
				std::vector<int> params = Utils::convertParamsToVector<int>(stringParams);
				p.colors[p.text.size()] = sf::Color(params[0], params[1], params[2]);
			}

			if (word.substr(0, 9) == "\\no_space")
				noSpace = true;
		}



		else {
			if (noSpace) {
				noSpace = false;
				p.text = p.text.substr(0, p.text.size() - 1);
				p.text += word;
			}
			else
				p.text += word + " ";
		}


	}

	paragraphes.push_back(p);

	if (paragraphes.size() > 0 && totalClearParagraphe == 0)
		totalClearParagraphe++;

	return true;
}

void UI::LongText::update(const sf::Time& deltaTime)
{
	calculateDimenstion();
	richText.setPosition(getGlobalPosition());
	richText.setCharacterSize(fontHeight * parent->getGlobalSize().y);
	richText.setLineSpacing(lineSpacing * parent->getGlobalSize().y);

	if (isStop)
		return;


	if (pp == nullptr && paragraphes.size() > 1)
		pp = &paragraphes[currParagraphe];

	//
	// Update pour les characters individuelles 
	//
	if (!isCharFinished) {
		timerChar += deltaTime;

		if (timerChar > pp->timePerChar) {
			char next = pp->text.at(charCounter);
			if (pp->colors.find(charCounter) != pp->colors.end())
				richText << pp->colors[charCounter];
			if (next == '\\') {
				charCounter++;
				richText << "\n";
			}
			else
				richText << pp->text.at(charCounter);

			charCounter++;
			timerChar = sf::Time::Zero;

			if (charCounter >= pp->text.size()) {
				if (!isAutomatic && pp->clear)
					isStop = true;
				isCharFinished = true;
				if (paragraphes.size() > currParagraphe + 1)
					currParagraphe++;
				else {
					isStop = true;
					done = true;
				}
				charCounter = 0;
			}
		}
	}
	//
	// update pour les fins de paragraphes
	//
	else {
		timer += deltaTime;

		if(pp != nullptr)
			if (pp->waitTime < timer) {
				timer = sf::Time::Zero;
				if (pp->clear) {
					richText.clear();
					currClearParagraphe++;
				}
				pp = &paragraphes[currParagraphe];
				isCharFinished = false;
			}
	}

}

unsigned int UI::LongText::getCurrClearParagrapheNumber()
{
	return currClearParagraphe;
}

unsigned int UI::LongText::totalClearParagrapheSize()
{
	return totalClearParagraphe;
}

void UI::LongText::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!hidden) {
		target.draw(richText);

		for (Widget* widget : childs) {
			if (widget)
				target.draw(*widget);
		}
	}
}

bool UI::LongText::loadFromFile(const std::string& path) {
	std::ifstream t(path);
	std::string str;
	t.seekg(0, std::ios::end);
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	return loadFromString(str);
}

void UI::LongText::setFont(const std::string& font_location)
{
	p_font = RessourceManager::FontManager::get(font_location);
}

void UI::LongText::setFont(std::shared_ptr<sf::Font> p_font)
{
	this->p_font = p_font;
}

std::shared_ptr<sf::Font> UI::LongText::getFont()
{
	return p_font;
}