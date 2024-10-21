#include "ChoiceBoxState.h"

ChoiceBoxState::ChoiceBoxState(Game* parent, std::shared_ptr<State> previous, sf::Vector2f position, sf::Vector2f size, UI::ShaderUIparam shaderParam, std::vector<UI::LabelButton*> choices, sf::Color textColor, bool parent_is_checkbox_to_be_removed)
	:State(parent),
	 gui(&parent->getWindow())
{
	this->previous = previous;
	choicesPanel = new UI::Panel("choicesPanel", position, size);
	choicesPanel->setShaderParams(shaderParam);
	defaultTextColor = textColor;
	this->parent_is_checkbox_to_be_removed = parent_is_checkbox_to_be_removed;
	for (UI::LabelButton* lb : choices) {
		if (lb) {
			this->choices.push_back(lb);
			choicesPanel->addWidget(this->choices.back());
		}
	}

	gui.add(choicesPanel);
	if (this->choices.size() > 0)
		gui.setSelection(this->choices.front());

}

std::string ChoiceBoxState::getType()
{
	return "choiceBox";
}

void ChoiceBoxState::init()
{
}

void ChoiceBoxState::handleEvent(sf::Event event)
{
}

void ChoiceBoxState::input(const sf::Time& deltaTime, std::vector<Config::InputAction> inputActions)
{
	gui.handleEvent(inputActions);
	gui.update(deltaTime);

	for (Config::InputAction inputAction : inputActions)
		if (inputAction == Config::InputAction::select) {
			float h = m_parent->getHeight();
			if (getParent()->peekState()->getId() == getId()) {
				if(parent_is_checkbox_to_be_removed)
					getParent()->changeState(static_pointer_cast<ChoiceBoxState>(this->previous)->previous, false);
				else
					getParent()->changeState(this->previous, false);
			}
		} 

}

void ChoiceBoxState::update(const sf::Time& deltaTime)
{

}

void ChoiceBoxState::draw(sf::RenderTarget& target, const sf::Time& deltaTime)
{
	previous->draw(getParent()->getWindow(), deltaTime);
	gui.render();
}

void ChoiceBoxState::setChoices(std::vector<UI::LabelButton*> choices)
{
	for (auto choice : choices)
		delete choice;
	choices.clear();
	this->choices = choices;
}

ChoiceBoxState::~ChoiceBoxState()
{
	choices.clear();
}
