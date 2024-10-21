#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>

namespace UI {

	class Widget: public sf::Drawable
	{
	public:
		Widget(const std::string& name, sf::Vector2f position, sf::Vector2f size, bool lockRatio = false);
		~Widget();

		virtual void update(const sf::Time& deltaTime) = 0;

		void addWidget(Widget* panel);
		Widget* getChild(int i);
		Widget* getChild(std::string id);
		Widget* getChildRecursive(std::string id);
		Widget* getParent();

		void remove(Widget* w);
		void removeAllAndDelete();

		sf::Vector2f getGlobalPosition() const;
		sf::Vector2f getGlobalOrigin() const;
		sf::Vector2f getGlobalSize() const;

		void hide();
		void show();
		bool isHidden();

		int childCount();

	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {};

		void calculateDimenstion();
		std::string name;
		bool hidden;
		bool lockRatio;
		bool longTextEnable;
		bool isButton{false};

		sf::Vector2f position;
		sf::Vector2f size;
		sf::Vector2f origin;
		sf::Vector2f screenSize;

		std::vector<Widget*> childs;

		sf::Time elapsedTime;

		Widget* parent;

		friend class Gui;
	};

}