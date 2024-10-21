#include "Button.h"


UI::Button::Button(const std::string& name, sf::Vector2f position, sf::Vector2f size, bool lockRatio)
	:Widget(name, position, size,lockRatio)
{
	isButton = true;
}

void UI::Button::connect(Action action, std::function<void()> functor)
{
	actions[action] = functor;
}

void UI::Button::call(Action action)
{
	actions[action]();
	lastAction = action;
}

UI::Action UI::Button::lastCall()
{
	return lastAction;
}

