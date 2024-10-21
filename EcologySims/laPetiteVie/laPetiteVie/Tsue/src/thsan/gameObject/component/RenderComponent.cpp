#include "RenderComponent.h"
#include <iostream>

void RenderComponent::addSpriteAnimationEvent(const std::string& animation, float at, bool call_once, std::function<void(GameObject&, Scene&, const sf::Time&)> func)
{
	for (auto& e : spriteAnimations) {
		if(e.second->hasAnimation(animation))
			spriteAnimationEventContainer[e.first].emplace_back<SpriteAnimationEvents>({ animation, at, call_once, false, std::move(func) });
	}
}

void RenderComponent::addSpriteAnimationEvent(const std::string& sprite_id, const std::string& animation, float at, bool call_once, std::function<void(GameObject&, Scene&, const sf::Time&)> func)
{
	if (spriteAnimations.count(sprite_id) > 0) {
		auto& sa = spriteAnimations[sprite_id];
		if (sa->hasAnimation(animation))
			spriteAnimationEventContainer[sprite_id].emplace_back<SpriteAnimationEvents>({ animation, at, call_once, false, std::move(func) });
	}
}

std::shared_ptr<SpriteAnimation> RenderComponent::createSpriteAnimation(const std::string& path, const std::string& sprite_id)
{
	std::shared_ptr<SpriteAnimation> sa = nullptr;

	if (spriteAnimations.count(sprite_id) == 0) {
		sa = std::make_shared<SpriteAnimation>(path);
		spriteAnimations[sprite_id] = sa;
		spriteAnimationEventContainer[sprite_id]; // this creates an element
	}
		
	return sa;
}

void RenderComponent::callEvent(Scene& scene, const sf::Time& dt)
{
	for (auto& sa_pair : spriteAnimations) {
		std::string sa_name = sa_pair.first;
		std::shared_ptr<SpriteAnimation> sa = sa_pair.second;
		auto& e = spriteAnimationEventContainer[sa_name];

		for (auto& spriteAnimationEvent : e)
			if (sa->getCurrAnimation() == spriteAnimationEvent.animation) {

				float total = sa->getCurrTimeDuration().asSeconds();
				float curr = sa->getCurrTimeReached().asSeconds();

				float t = curr / total;
				if (t > 1.0f)
					t = 1.0f;
				else if (t <= 0.0f)
					t = 0.f;

				if (t >= spriteAnimationEvent.at) {
					spriteAnimationEvent.func(*parent, scene, dt);
					spriteAnimationEvent.was_called = true;
				}
			}

		if (e.size() != 0) {
			auto it = std::remove_if(e.begin(), e.end(), [](const SpriteAnimationEvents& se) {
				if (se.call_once && se.was_called)
					return true;
				else
					return false;
				});

			e.erase(it, e.end());
		}
	}
}
