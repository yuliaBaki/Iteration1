#include "VolumetricPostProcessingPhysicComponent.h"
#include "../../CollisionEngine/CollisionEngine.h"
#include "../../util/helperMath.h"
#include <iostream>

VolumetricPostProcessingPhysicComponent::VolumetricPostProcessingPhysicComponent(GameObject* target):
	target(target)
{
}

void VolumetricPostProcessingPhysicComponent::init(Scene& scene)
{
	if (!parent->hasComponent(COMPONENT_DATA::HITBOX)) {
		hitbox = new HitBox();
		parent->setComponent(COMPONENT_DATA::HITBOX, hitbox);
	}
	else
		hitbox = parent->getComponent<HitBox>(COMPONENT_DATA::HITBOX);

	if (!parent->hasComponent(COMPONENT_DATA::VOLUME_POST_PROCESS)) {
		vol_pp_data = new VolumetricPostProcessData();
		parent->setComponent(COMPONENT_DATA::VOLUME_POST_PROCESS, vol_pp_data);
	}
	else
		vol_pp_data = parent->getComponent<VolumetricPostProcessData>(COMPONENT_DATA::VOLUME_POST_PROCESS);

	transform = parent->getComponent<Transform>(COMPONENT_DATA::TRANSFORM);

	pp = scene.getPostProcess();
	CollisionEngine::add(parent, transform, hitbox);
}

void VolumetricPostProcessingPhysicComponent::update(Scene& scene, const sf::Time& dt)
{
	if (!locked) {
		if (target) {
			if (CollisionEngine::overlap(parent, target) && !isOverlapping) {
				isOverlapping = true;
				for (const auto& pair : vol_pp_data->effects_and_opacity) {
					if (isFirstInterlapsing || isInterpolationFullyDone) {
						default_opacity[pair.first] = pp->getOpacity(pair.first);
						isFirstInterlapsing = false;
						isInterpolationFullyDone = false;
					}
					enable_then_init_interpolate(pair.first, pair.second,
								vol_pp_data->ease, vol_pp_data->interpolation,
								vol_pp_data->in_interpolation_duration);
				}
			}
			else if (!CollisionEngine::overlap(parent, target) && isOverlapping) {
				isOverlapping = false;
				for (const auto& pair : vol_pp_data->effects_and_opacity) {
					init_interpolate_then_disable(pair.first,
						vol_pp_data->ease, vol_pp_data->interpolation,
						vol_pp_data->out_interpolation_duration);
				}
			}
		}

		interpolate_effect(dt);
	}
}

void VolumetricPostProcessingPhysicComponent::interpolate_effect(const sf::Time& dt)
{
	static bool isLastWhereElapsedEqualDuration = false;

	if (!isInterpolating) {
		return;
	}
	
	if (elapsed_time > duration) {
		isLastWhereElapsedEqualDuration = true;
		elapsed_time = duration;
	}

	float t{1.0};

	if (duration != sf::Time::Zero)
		t = interpolate<float>(ease, interpolate_type, elapsed_time.asSeconds() / duration.asSeconds());
	else 
		isInterpolating = false;
	
	static int count = 0;
	count++;

	for (auto& val : curr_opacity) {
		val.second = (1.0f - t) * begin_opacity[val.first] + t * final_opacity[val.first];
		pp->setOpacity(val.first, val.second);

	}

	if (isLastWhereElapsedEqualDuration) {
		isLastWhereElapsedEqualDuration = false;
		isInterpolating = false;
		isInterpolationFullyDone = true;
		elapsed_time = sf::Time::Zero;
	}
	else
		this->elapsed_time += dt;
}

void VolumetricPostProcessingPhysicComponent::init_interpolate(PostProcess::Effect effect, float final_opacity, Ease e, InterpolationType it, sf::Time duration)
{
	begin_opacity[effect] = pp->getOpacity(effect);
	curr_opacity[effect] = pp->getOpacity(effect);
	this->final_opacity[effect] = final_opacity;
	this->ease = ease;
	this->elapsed_time = sf::Time::Zero;
	interpolate_type = it;
	this->duration = duration;
	isInterpolating = true;
}

void VolumetricPostProcessingPhysicComponent::enable_then_init_interpolate(PostProcess::Effect effect, float final_opacity, Ease e, InterpolationType it, sf::Time duration)
{
	init_interpolate(effect, final_opacity, e, it, duration);
	if (!pp->isEnable()) { 
		started_enable[effect] = false;
		pp->enable_effect(effect, curr_opacity[effect]);
	}else
		started_enable[effect] = true;

	isMarkedForDisabling = false;
}


void VolumetricPostProcessingPhysicComponent::init_interpolate_then_disable(PostProcess::Effect effect, Ease e, InterpolationType it, sf::Time duration)
{
	init_interpolate(effect, default_opacity[effect], e, it, duration);
	if(!started_enable[effect])
		isMarkedForDisabling = true;
	else
		isMarkedForDisabling = false;
}

bool VolumetricPostProcessingPhysicComponent::locked = false;
