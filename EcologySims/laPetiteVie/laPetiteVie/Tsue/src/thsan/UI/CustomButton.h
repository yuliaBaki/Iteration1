#pragma once

#include "Widget.h"
#include "Button.h"


namespace UI {
	class CustomButton : public Button
	{
	public:
		CustomButton(const std::string& name, sf::Vector2f position = sf::Vector2f(0.f, 0.f), sf::Vector2f size = sf::Vector2f(1.0,1.0));
		~CustomButton() = default;

		void create(std::function<void(sf::VertexArray&)> functor);
		void onUpdate(std::function<void(sf::VertexArray&, sf::Time deltaTime)> functor);
		void update(const sf::Time& deltaTime) override;

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		sf::Time elapsedTime;
		sf::VertexArray vertices;
		sf::VertexArray final_vertices;
		std::vector<sf::Vector2f> normalizedPosition;

		std::function<void(sf::VertexArray&, sf::Time ElapsedTime)> verticesUpdate;
	};
}

