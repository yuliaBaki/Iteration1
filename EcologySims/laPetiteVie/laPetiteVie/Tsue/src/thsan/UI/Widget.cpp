#include "Widget.h"
#include "../ressourceManager/FontManager.h"

UI::Widget::~Widget()
{
	for (Widget* p : childs)
		delete p;
}

UI::Widget::Widget(const std::string& name, sf::Vector2f position, sf::Vector2f size, bool lockRatio):
	hidden(true),
	name(name)
{
	elapsedTime = sf::seconds(0);
	this->position = position;
	this->size = size;
	this->origin = sf::Vector2f(0, 0);
	this->lockRatio = lockRatio;
	show();
}

UI::Widget* UI::Widget::getChild(std::string id)
{
	auto child = std::find_if(childs.begin(), childs.end(), [&id](Widget* widget) {return widget->name == id; });
	if (child != childs.end()) {
		int i = std::distance(childs.begin(), child);
		return childs.at(i);
	}
	else
		return nullptr;
}

UI::Widget* UI::Widget::getChildRecursive(std::string id)
{
	Widget* w = getChild(id);

	if (!w)
		for (Widget* c : childs) {
			w = c->getChildRecursive(id);
			if (w)
				return w;
		}

	return w;
}

UI::Widget* UI::Widget::getParent()
{
	return parent;
}

void UI::Widget::remove(Widget* w)
{
	auto it = std::find(childs.begin(), childs.end(), w);
	if(it != childs.end())
		childs.erase(it);
}

sf::Vector2f UI::Widget::getGlobalPosition() const
{
	return origin + sf::Vector2f(position.x * screenSize.x, position.y * screenSize.y);
}

sf::Vector2f UI::Widget::getGlobalOrigin() const
{
	return origin;
}

sf::Vector2f UI::Widget::getGlobalSize() const
{
	return sf::Vector2f(screenSize.x * size.x, screenSize.y * size.y);
}

void UI::Widget::update(const sf::Time& deltaTime)
{
	elapsedTime += deltaTime;
	for (Widget* panel : childs)
		panel->update(deltaTime);
}

void UI::Widget::addWidget(Widget* widget)
{
	widget->parent = this;
	childs.push_back(widget);
}

UI::Widget* UI::Widget::getChild(int i)
{
	if (childs.size() > i)
		return childs.at(i);
	return nullptr;
}

void UI::Widget::hide()
{
	hidden = true;
}

void UI::Widget::show()
{
	hidden = false;
}

bool UI::Widget::isHidden()
{
	return hidden;
}

int UI::Widget::childCount()
{
	return childs.size();
}

void UI::Widget::calculateDimenstion()
{
	if (parent) {
		screenSize.x = parent->screenSize.x * parent->size.x;
		screenSize.y = parent->screenSize.y * parent->size.y;

		origin.x = parent->origin.x + parent->position.x * parent->screenSize.x;
		origin.y = parent->origin.y + parent->position.y * parent->screenSize.y;
	}


	if (lockRatio)
		screenSize.y = (size.y / size.x) * screenSize.x;
}
