#pragma once

#include  <thsan/state/State.h>
#include  <thsan/UI/LongText.h>
#include  <thsan/UI/Panel.h>
#include  <thsan/UI/LabelButton.h>
#include  <thsan/UI/CustomButton.h>
#include  <thsan/UI/Gui.h>
#include "../choiceBox/ChoiceBoxState.h"
#include <map>

class DialogueState :	public State
{
	public:
		DialogueState(Game* parent, std::shared_ptr<State> previous, sf::Vector2f position, sf::Vector2f size, const std::string& path, UI::ShaderUIparam shaderParam, sf::Color textColor = sf::Color::White, float fontScreenPercentageSize = 0.1f, float lineSpacing = 0.0f);
		std::string getType() override;
		void init() override;
		void handleEvent(sf::Event event) override;
		void input(const sf::Time &deltaTime, std::vector<Config::InputAction> inputActions) override;
		void update(const sf::Time &deltaTime) override;
		void draw(sf::RenderTarget& target, const sf::Time &deltaTime) override;

		void addChoiceBox(std::vector<UI::LabelButton*> choices, unsigned int paragrapheNumber, sf::Vector2f position, sf::Vector2f size);
		~DialogueState() = default;

    private:
		UI::Gui gui;
		UI::Panel* dialoguePanel;
		UI::LongText* dialogueText;
		UI::CustomButton* dialogueButtonNext;


		std::map<int, std::shared_ptr<ChoiceBoxState>> choiceBoxes;

		sf::Color defaultTextColor;
		std::shared_ptr<State> previous;
};

