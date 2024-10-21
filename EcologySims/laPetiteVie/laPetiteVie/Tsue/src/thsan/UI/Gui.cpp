#include "Gui.h"

UI::Gui::Gui(sf::RenderWindow* window):
	selection(nullptr)
{
	p_window = window;
}

UI::Gui::~Gui()
{
	for (Widget* w : widgets)
		delete w;
	widgets.clear();

	//tous les bouttons et même selections sont dans widget.
}

bool UI::Gui::handleEvent(std::vector<Config::InputAction> inputActions)
{

	if (!selection)
		return false;

	//retirer le static
	static bool released = true;

	if(inputActions.size() > 0)
		for (Config::InputAction inputAction : inputActions) {

			if (inputAction == Config::InputAction::up) {
				if (released) {
					if(selection->up){
						selection->call(UI::Action::crossed);
						selection = selection->up;
					}
					released = false;
					return true;
				}
			}
			else if (inputAction == Config::InputAction::down) {
				if (released) {
					if (selection->down) {
						selection->call(UI::Action::crossed);
						selection = selection->down;
					}
					released = false;
					return true;
				}
			}
			else if (inputAction == Config::InputAction::right) {
				if (released) {
					if (selection->right) {
						selection->call(UI::Action::crossed);
						selection = selection->right;
					}
					released = false;
					return true;
				}
			}
			else if (inputAction == Config::InputAction::left) {
				if (released) {
					if (selection->left) {
						selection->call(UI::Action::crossed);
						selection = selection->left;
					}
					released = false;
					return true;
				}
			}
			else if (inputAction == Config::InputAction::select) {
				if (released) {

					selection->call(UI::Action::pressed);
					released = false;
					return true;
				}
			}
			else {
				released = true;
			}
		}
	else {
		released = true;
	}

	if(selection->lastCall() != UI::Action::hover)
		selection->call(UI::Action::hover);

	return true;
}

void UI::Gui::update(const sf::Time deltaTime)
{
	for (Widget* w : widgets) {

		w->origin = sf::Vector2f(0, 0);
		w->screenSize = sf::Vector2f(p_window->getSize().x, p_window->getSize().y);

		w->update(deltaTime);
	}
}

void UI::Gui::render()
{
	for (Widget* w : widgets)
		p_window->draw(*w);
}

template <typename Functor>
void UI::Gui::onAllWidget(Widget* root, Functor action) {
	action(root);
	for (Widget* w : root->childs)
		onAllWidget(w, action);
}

void UI::Gui::add(Widget* widget)
{
	widgets.push_back(widget);

	onAllWidget(widget, [this](Widget* widget) {
		if (widget->isButton) {
			buttons.push_back(static_cast<Button*>(widget));
		}
	});
}

void UI::Gui::remove(Widget* widget)
{
	if (widget->isButton)
		buttons.erase(std::find(buttons.begin(), buttons.end(), static_cast<Button*>(widget)));

	auto it = std::find(widgets.begin(), widgets.end(), widget);

	if (it != widgets.end())
		widgets.erase(it);
	else
		for (Widget* root : widgets) 
			onAllWidget(root, [this, &widget](Widget* w) {
				if (w == widget) {
					Widget* parent = w->getParent();
					if (parent)
						parent->remove(w);
				}
				});

}

void UI::Gui::refreshButtons()
{
	for (Widget* root : widgets)
		onAllWidget(root, [this](Widget* w) {
		if (w->isButton) {
			Button* b = static_cast<Button*>(w);
			if (std::find(buttons.begin(), buttons.end(), b) == buttons.end())
				buttons.push_back(b);
		}
			});

}

void UI::Gui::refreshButtons(Widget* widget)
{
	onAllWidget(widget, [this](Widget* w) {
		if (w->isButton) {
			Button* b = static_cast<Button*>(w);
			if (std::find(buttons.begin(), buttons.end(), b) == buttons.end())
				buttons.push_back(b);
		}
		});
}

bool UI::Gui::setSelection(Button* button)
{

	for (Button* b : buttons) {
		if (b == button) {
			selection = button;
			return true;
		}
	}

	return false;
}

UI::Button* UI::Gui::getSelection()
{
	return selection;
}
