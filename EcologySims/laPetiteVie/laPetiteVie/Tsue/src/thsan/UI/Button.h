#pragma once
#include "Widget.h"
#include "../state/State.h"
#include "../scene/Scene.h"

#include <functional>

namespace UI{
	enum class Action {
		nothing,
		pressed,
		hover,
		crossed
	};


	class Button: public Widget
	{
	public:
		Button(const std::string& name, sf::Vector2f position, sf::Vector2f size, bool lockRatio = false);
		void connect(Action action, std::function<void()> functor);
		void call(Action action);

		virtual void update(const sf::Time& deltaTime) = 0;

		Action lastCall();

		Button* up;
		Button* down;
		Button* left;
		Button* right;
	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

		std::unordered_map<Action, std::function<void()>> actions;
		Action lastAction{Action::nothing};


	};


}