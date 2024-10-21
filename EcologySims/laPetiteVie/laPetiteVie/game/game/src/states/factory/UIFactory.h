#pragma once

#include <thsan/state/State.h>
#include <thsan/gameObject/GameObject.h>

class UIFactory {

public:
	UIFactory(std::shared_ptr<State> current);
	~UIFactory() = default;
	std::shared_ptr<State> createPlayerActionChoiceBox(GameObject& player, Scene& scene);
	std::shared_ptr<State> createPlayerListChoiceBox(GameObject& player, Scene& scene);

private:
	std::shared_ptr<State> current;
	std::string playerListChoiceBoxId{""};
};