#include "DialogueState.h"
#include <sstream>
#include <thsan/ressourceManager/StateManager.h>

DialogueState::DialogueState(Game* parent, std::shared_ptr<State> previous, sf::Vector2f position, sf::Vector2f size, const std::string& path, UI::ShaderUIparam shaderParam, sf::Color textColor, float fontScreenPercentageSize, float lineSpacing):
	State(parent),
	gui(&parent->getWindow())
{
	this->previous = previous;

	dialoguePanel = new UI::Panel("dialoguePanel", position, size);
	dialogueText = new UI::LongText("dialogueText", "media/font/Final_Fantasy_VII/Final_Fantasy_VII.ttf", sf::Vector2f(0.1, 0.0), path, fontScreenPercentageSize, lineSpacing);
	dialogueButtonNext = new UI::CustomButton("dialogueButtonNext", sf::Vector2f(0.8, 0.8), sf::Vector2f(0.05, 0.2));

	dialoguePanel->setShaderParams(shaderParam);
	defaultTextColor = textColor;

	dialogueButtonNext->hide();
	dialogueButtonNext->connect(UI::Action::pressed, [&]() {
		dialogueText->resume();
		dialogueButtonNext->hide();
		gui.setSelection(nullptr);
		});
	dialogueButtonNext->connect(UI::Action::hover, [&]() {

		});

	dialogueButtonNext->create([&](sf::VertexArray& vertices) {
		vertices.resize(6);
		vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
		vertices[3].position = sf::Vector2f(0.f,0.f);
		vertices[4].position = sf::Vector2f(1.f, 0.f);
		vertices[5].position = sf::Vector2f(0.5f, 0.3f);
		vertices[0].position = sf::Vector2f(0.f, 0.1f);
		vertices[1].position = sf::Vector2f(1.f, 0.1f);
		vertices[2].position = sf::Vector2f(0.5f, 0.4f);

		vertices[0].color = shaderParam.colorBorderBorders;
		vertices[1].color = shaderParam.colorBorderBorders;
		vertices[2].color = shaderParam.colorBorderBorders;
		vertices[3].color = shaderParam.colorBorder;
		vertices[4].color = shaderParam.colorBorder;
		vertices[5].color = shaderParam.colorBorder;
		});

	dialogueButtonNext->onUpdate([&](sf::VertexArray& vertices, sf::Time deltaTime) {
		static sf::Time dt = sf::Time::Zero;
		static float y_tr = 0.01;
		static float dir = 1.0;
		dt += deltaTime * dir;

		if (dt > sf::seconds(0.2))
			dir = -1.0;

		else if (dt < sf::seconds(0.0))
			dir = 1.0;

		vertices[3].position.y += y_tr * dir;
		vertices[4].position.y += y_tr * dir;
		vertices[5].position.y += y_tr * dir;

		});

	for (int i = 0; i < 4; i++)
		dialoguePanel->setBorderSize(i, shaderParam.borderSize[i]);
	for (int i = 0; i < 4; i++)
		dialoguePanel->setCornerRadius(i, shaderParam.cornerRadius[i]);

	dialoguePanel->addWidget(dialogueText);
	dialoguePanel->addWidget(dialogueButtonNext);
	gui.add(dialoguePanel);
}

std::string DialogueState::getType()
{
	return "dialogue";
}

void DialogueState::init()
{
}

void DialogueState::handleEvent(sf::Event event)
{

}

void DialogueState::input(const sf::Time& deltaTime, std::vector<Config::InputAction> inputActions)
{
	gui.handleEvent(inputActions);
}

void DialogueState::update(const sf::Time& deltaTime)
{
	if (dialogueText->isParagraphefinished()) {
		int currParagraphe = dialogueText->getCurrClearParagrapheNumber();
		if (choiceBoxes.find(currParagraphe) != choiceBoxes.end()) {
			if (choiceBoxes[currParagraphe]) {
				getParent()->changeState(choiceBoxes[currParagraphe], true);
				choiceBoxes[currParagraphe] = nullptr;
			}
			else
				dialogueText->resume();

		}
		else {
			gui.setSelection(dialogueButtonNext);
			dialogueButtonNext->show();
		}
	}

	gui.update(deltaTime);
	previous->update(deltaTime);

	if (dialogueText->isFinished()) {
		getParent()->changeState(this->previous, false);
	}
}

void DialogueState::draw(sf::RenderTarget& target, const sf::Time& deltaTime)
{
	previous->draw(getParent()->getWindow(), deltaTime);
	gui.render();
}

void DialogueState::addChoiceBox(std::vector<UI::LabelButton*> choices, unsigned int paragrapheNumber, sf::Vector2f position, sf::Vector2f size)
{
	choiceBoxes[paragrapheNumber] = RessourceManager::StateManager::create<ChoiceBoxState>(
		getParent(),
		RessourceManager::StateManager::get<State>(this->getId()),
		position,
		size,
		dialoguePanel->getShaderParams(),
		choices,
		defaultTextColor
	);
}
