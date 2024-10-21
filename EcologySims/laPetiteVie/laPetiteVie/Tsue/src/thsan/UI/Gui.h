#pragma once
#include "../config/ControlSetting.h"
#include "Widget.h"
#include "Button.h"
#include <iostream>


namespace UI {
	class Gui
	{
	public:
		Gui(sf::RenderWindow* window);
		~Gui();
		bool handleEvent(std::vector<Config::InputAction> inputActions);
		void update(const sf::Time deltaTime);
		void render();

		void add(Widget* widget);

		//remove will also remove any childs inside widgets.
		void remove(Widget* widget);

		//pointers to widget that are also buttons aren't automatically made
		void refreshButtons();
		void refreshButtons(Widget* widget);


		bool setSelection(Button* button); // O(n)
		Button* getSelection();


	private:
		sf::RenderWindow* p_window;
		std::vector<Widget*> widgets;
		std::vector<Button*> buttons;
		Button* selection;
		template<typename Functor>
		void onAllWidget(Widget* root, Functor action);
	};
}
