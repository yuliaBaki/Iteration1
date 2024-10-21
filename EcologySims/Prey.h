#pragma once

#include "Animal.h"

class Prey : public Animal {
public:
	Prey(std::shared_ptr<std::vector<sf::Texture>> gifFrames_, const sf::Vector2f& targetPos_,
		const sf::Sprite& objectSprite_, float life_, float hunger_, bool isFlipped_, const sf::Vector2f& moveDirection_, bool moving_,
		const sf::RectangleShape& hungerBar_, const sf::RectangleShape& lifeBar_)
		: Animal(gifFrames_, targetPos_, objectSprite_, life_, hunger_, isFlipped_, moveDirection_, moving_, hungerBar_, lifeBar_) {

	};

	Prey(const Prey& other) :Animal(other) {

	}

	bool isPrey() override {
		return true;
	}

	~Prey() = default;
};