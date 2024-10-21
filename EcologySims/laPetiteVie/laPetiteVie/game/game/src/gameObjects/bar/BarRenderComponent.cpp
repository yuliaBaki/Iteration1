#include "BarRenderComponent.h"
#include <thsan/util/helperMath.h>
#include <iostream>

BarRenderComponent::BarRenderComponent()
{
}


BarRenderComponent::~BarRenderComponent()
{
}


void BarRenderComponent::init(Scene& scene)
{
	if (parent->hasComponent(COMPONENT_DATA::BARS))
		bars_data = parent->getComponent<BarsData>(COMPONENT_DATA::BARS);
	else
	{
		bars_data = new BarsData();
		parent->setComponent(COMPONENT_DATA::BARS, bars_data);
	}

	bars_data->show_bar = false;
	bars_data->bar_screen_ratio_top = 0.1f;
	bars_data->bar_screen_ratio_bottom = 0.1f;


}

void BarRenderComponent::update(Scene& scene, const sf::Time& dt, sf::RenderTarget& target, RendererPseudo3D& renderer)
{
	sf::Vector2f size = scene.getRenderResolution();
	sf::Color kinda_ambient = scene.getDirectionalLightProperties().ambient;
	kinda_ambient.a = 168;
	top_bar.setFillColor(kinda_ambient);
	bottom_bar.setFillColor(kinda_ambient);

	if (bars_data->show_bar && !enable_start) {
		enable_start = true;
		enable_end = false;
		is_currently_shown = true;
	}
	else if(!bars_data->show_bar && is_currently_shown) {
		enable_end = true;
		enable_start = false;
	}

	if (elapsedTime < sf::seconds(0.f))
		elapsedTime = sf::seconds(0.f);

	if (elapsedTime <= sf::seconds(0.f) && enable_end) {
		enable_end = false;
		is_currently_shown = false;
	}

	if (elapsedTime > bars_data->duration && enable_start)
		enable_start = false;

	if (enable_start) {
		elapsedTime += dt;
		float s = 1.f;
		if(elapsedTime < bars_data->duration)
			s = interpolate(Ease::inOut, InterpolationType::cubic, elapsedTime.asSeconds() / bars_data->duration.asSeconds());

		float rect_size_y_bottom = size.y * bars_data->bar_screen_ratio_bottom * s;
		float rect_size_y_top = size.y * bars_data->bar_screen_ratio_top * s;
		top_bar.setSize(sf::Vector2f(size.x, rect_size_y_top));
		bottom_bar.setSize(sf::Vector2f(size.x, rect_size_y_bottom));

		top_bar.setPosition(sf::Vector2f(0.f, 0.f));
		bottom_bar.setPosition(sf::Vector2f(0.f, size.y - rect_size_y_bottom));
	}

	else if (enable_end) {
		elapsedTime -= dt * bars_data->end_speed;
		float s = 0.f;
		if (elapsedTime > sf::Time::Zero && elapsedTime < bars_data->duration)
			s = interpolate(Ease::inOut, InterpolationType::cubic, elapsedTime.asSeconds() / bars_data->duration.asSeconds());


		float rect_size_y_bottom = size.y * bars_data->bar_screen_ratio_bottom * s;
		float rect_size_y_top = size.y * bars_data->bar_screen_ratio_top * s;
		top_bar.setSize(sf::Vector2f(size.x, rect_size_y_top));
		bottom_bar.setSize(sf::Vector2f(size.x, rect_size_y_bottom));

		top_bar.setPosition(sf::Vector2f(0.f, 0.f));
		bottom_bar.setPosition(sf::Vector2f(0.f, size.y - rect_size_y_bottom));
	}

	renderer.add_on_top_without_perspective(&top_bar, 0);
	renderer.add_on_top_without_perspective(&bottom_bar, 0);
}
