#include "CustomButton.h"
#include <iostream>

UI::CustomButton::CustomButton(const std::string& name, sf::Vector2f position, sf::Vector2f size)
	: Button(name, position, size, false),
	elapsedTime(sf::Time::Zero)
{

}

void UI::CustomButton::create(std::function<void(sf::VertexArray&)> functor)
{
	functor(vertices);

	normalizedPosition.clear();

	final_vertices.resize(vertices.getVertexCount());
	final_vertices.setPrimitiveType(vertices.getPrimitiveType());
	for (int i = 0; i < vertices.getVertexCount(); i++) {
		normalizedPosition.push_back(vertices[i].position);
		final_vertices[i] = vertices[i];
	}
}

void UI::CustomButton::onUpdate(std::function<void(sf::VertexArray&, sf::Time deltaTime)> functor)
{
	verticesUpdate = functor;
}

void UI::CustomButton::update(const sf::Time& deltaTime)
{
	elapsedTime += deltaTime;

	if (verticesUpdate) {
		verticesUpdate(vertices, deltaTime);
		normalizedPosition.clear();

		final_vertices.resize(vertices.getVertexCount());
		final_vertices.setPrimitiveType(vertices.getPrimitiveType());
		for (int i = 0; i < vertices.getVertexCount(); i++) {
			normalizedPosition.push_back(vertices[i].position);
			final_vertices[i] = vertices[i];
		}
	}
	calculateDimenstion();

	sf::Vector2f global_position = getGlobalPosition();
	sf::Vector2f global_size = getGlobalSize();

	for (int i = 0; i < vertices.getVertexCount(); i++) {
		final_vertices[i].position.x = global_position.x + normalizedPosition[i].x * global_size.x;
		final_vertices[i].position.y = global_position.y + normalizedPosition[i].y * global_size.y;
	}

}

void UI::CustomButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!hidden) {
		target.draw(final_vertices);

		for (Widget* widget : childs) {
			if (widget)
				target.draw(*widget);
		}
	}
}
