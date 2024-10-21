#include "ControlSetting.h"
#include <iostream>

void Config::ControlSetting::add(sf::Keyboard::Key inputKey, InputState inputState, InputAction inputAction)
{
	std::pair<sf::Keyboard::Key, InputState> key{ inputKey, inputState };
	std::vector<InputAction>* value = &inputMap[key];

	auto it = std::find(value->begin(), value->end(), inputAction);
	if (it == value->end())
		value->push_back(inputAction);

	inputActive[key] = false;
}

void Config::ControlSetting::remove(sf::Keyboard::Key inputKey, InputState inputState, InputAction inputAction)
{
	std::pair<sf::Keyboard::Key, InputState> key{ inputKey, inputState };
	std::vector<InputAction>* value = &inputMap[key];

	auto it = std::find(value->begin(), value->end(), inputAction);
	if (it != value->end())
		value->erase(std::find(value->begin(), value->end(), inputAction));

	inputActive.erase(key);
}


void Config::ControlSetting::handleInput(sf::Event event)
{
	/*
	* todo: double click.
	* 500ms.
	* sauvegarde tous les keys pressed pendant 0.500ms
	* s'il y en a un autre
	* ->double click
	* comment negate un double click?
	*/

	std::vector<InputAction> temp_values;
	
	for (auto kv : inputActive) {
		if (kv.first.first == event.key.code) {
			if (event.type == sf::Event::KeyReleased) {
				if(kv.first.second == InputState::isPressed)
					inputActive[kv.first] = false;
				else if(kv.first.second == InputState::isReleased)
					inputActive[kv.first] = true;
			}
			else if (event.type == sf::Event::KeyPressed) {
				inputActive[kv.first] = true;
			}
		}

	}
}

void Config::ControlSetting::updateInput()
{
	values.clear();
	for (auto kv : inputActive) {
		if (kv.second)
			for (auto v : inputMap[kv.first])
				values.push_back(v);
	}

	for (auto kv : inputActive) {
		if (kv.first.second == InputState::isPressedNoRepeat)
			inputActive[kv.first] = false;
		
		else if (kv.first.second == InputState::isReleased)
			inputActive[kv.first] = false;
	}
}

std::vector<Config::InputAction> Config::ControlSetting::getInput()
{
	return values;
}
