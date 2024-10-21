#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/easing.hpp>
#include <unordered_map>
#include <map>
#include <functional>
/*
* Based on unity3D source
* https://github.com/Unity-Technologies/UnityCsReference/blob/61f92bd79ae862c4465d35270f9d1d57befd1761/Runtime/Export/Math/Mathf.cs#L288
*/
glm::vec3 SmoothDamp(glm::vec3 current, glm::vec3 target, glm::vec3& currentVelocity, float smoothTime, float maxSpeed, float deltaTime);
float SmoothDamp(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed, float deltaTime);
float SmoothDamp(float current, float target, float& currentVelocity, float smoothTime, float deltaTime);
float SmoothDamp(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed, float deltaTime);

enum class Ease {
	in,
	out,
	inOut,
	None
};

enum class InterpolationType {
	back,
	bounce,
	circular,
	cubic,
	elastic,
	exponential,
	linear,
	quadratic,
	quintic,
	sine
};

struct pair_interpolation_hash
{
	std::size_t operator() (const std::pair<Ease, InterpolationType>& pair) const {
		return std::hash<Ease>()(pair.first) ^ std::hash<InterpolationType>()(pair.second);
	}
};

template <typename T>
float interpolate(Ease ease, InterpolationType interpolation_type, const T& t) {

	const static std::unordered_map<std::pair<Ease, InterpolationType>, std::function<float(const float&)>, pair_interpolation_hash> interpolation_func
	{
		{{Ease::in, InterpolationType::back}		, [](const T& t) { return glm::backEaseIn<T>(t); }},
		{{Ease::in, InterpolationType::bounce}		, [](const T& t) { return glm::bounceEaseIn<T>(t); }},
		{{Ease::in, InterpolationType::circular}	, [](const T& t) { return glm::circularEaseIn<T>(t); }},
		{{Ease::in, InterpolationType::cubic}		, [](const T& t) { return glm::cubicEaseIn<T>(t); }},
		{{Ease::in, InterpolationType::elastic}		, [](const T& t) { return glm::elasticEaseIn<T>(t); }},
		{{Ease::in, InterpolationType::exponential}	, [](const T& t) { return glm::exponentialEaseIn<T>(t); }},
		{{Ease::in, InterpolationType::linear}		, [](const T& t) { return glm::linearInterpolation<T>(t); }},
		{{Ease::in, InterpolationType::quadratic}	, [](const T& t) { return glm::quadraticEaseIn<T>(t); }},
		{{Ease::in, InterpolationType::quintic}		, [](const T& t) { return glm::quinticEaseIn<T>(t); }},
		{{Ease::in, InterpolationType::sine}		, [](const T& t) { return glm::sineEaseIn<T>(t); }},
		{{Ease::inOut, InterpolationType::back}		, [](const T& t) { return glm::backEaseIn<T>(t); }},
		{{Ease::inOut, InterpolationType::bounce}		, [](const T& t) { return glm::bounceEaseInOut<T>(t); }},
		{{Ease::inOut, InterpolationType::circular}		, [](const T& t) { return glm::circularEaseInOut<T>(t); }},
		{{Ease::inOut, InterpolationType::cubic}		, [](const T& t) { return glm::cubicEaseInOut<T>(t); }},
		{{Ease::inOut, InterpolationType::elastic}		, [](const T& t) { return glm::elasticEaseInOut<T>(t); }},
		{{Ease::inOut, InterpolationType::exponential}	, [](const T& t) { return glm::exponentialEaseInOut<T>(t); }},
		{{Ease::inOut, InterpolationType::linear}		, [](const T& t) { return glm::linearInterpolation<T>(t); }},
		{{Ease::inOut, InterpolationType::quadratic}	, [](const T& t) { return glm::quadraticEaseInOut<T>(t); }},
		{{Ease::inOut, InterpolationType::quintic}		, [](const T& t) { return glm::quinticEaseInOut<T>(t); }},
		{{Ease::inOut, InterpolationType::sine}			, [](const T& t) { return glm::sineEaseInOut<T>(t); }},
		{{Ease::out, InterpolationType::back}		, [](const T& t) { return glm::backEaseOut<T>(t); }},
		{{Ease::out, InterpolationType::bounce}		, [](const T& t) { return glm::bounceEaseOut<T>(t); }},
		{{Ease::out, InterpolationType::circular}	, [](const T& t) { return glm::circularEaseOut<T>(t); }},
		{{Ease::out, InterpolationType::cubic}		, [](const T& t) { return glm::cubicEaseOut<T>(t); }},
		{{Ease::out, InterpolationType::elastic}	, [](const T& t) { return glm::elasticEaseOut<T>(t); }},
		{{Ease::out, InterpolationType::exponential}, [](const T& t) { return glm::exponentialEaseOut<T>(t); }},
		{{Ease::out, InterpolationType::linear}		, [](const T& t) { return glm::linearInterpolation<T>(t); }},
		{{Ease::out, InterpolationType::quadratic}	, [](const T& t) { return glm::quadraticEaseOut<T>(t); }},
		{{Ease::out, InterpolationType::quintic}	, [](const T& t) { return glm::quinticEaseOut<T>(t); }},
		{{Ease::out, InterpolationType::sine}		, [](const T& t) { return glm::sineEaseOut<T>(t); }},
		{{Ease::None, InterpolationType::back}			, [](const T& t) { return glm::linearInterpolation<T>(t); }},
		{{Ease::None, InterpolationType::bounce}		, [](const T& t) { return glm::linearInterpolation<T>(t); }},
		{{Ease::None, InterpolationType::circular}		, [](const T& t) { return glm::linearInterpolation<T>(t); }},
		{{Ease::None, InterpolationType::cubic}			, [](const T& t) { return glm::linearInterpolation<T>(t); }},
		{{Ease::None, InterpolationType::elastic}		, [](const T& t) { return glm::linearInterpolation<T>(t); }},
		{{Ease::None, InterpolationType::exponential}	, [](const T& t) { return glm::linearInterpolation<T>(t); }},
		{{Ease::None, InterpolationType::linear}		, [](const T& t) { return glm::linearInterpolation<T>(t); }},
		{{Ease::None, InterpolationType::quadratic}		, [](const T& t) { return glm::linearInterpolation<T>(t); }},
		{{Ease::None, InterpolationType::quintic}		, [](const T& t) { return glm::linearInterpolation<T>(t); }},
		{{Ease::None, InterpolationType::sine}			, [](const T& t) { return glm::linearInterpolation<T>(t); }}
	};

	return interpolation_func.at({ ease, interpolation_type })(t);
}

/*
template <typename T>
T interpolate(Ease ease, InterpolationType interpolation_type, T t) {

	float result = 1.f;

	if (ease == Ease::in){
		if (interpolation_type == InterpolationType::back)
			result = glm::backEaseIn(t);
		else if (interpolation_type == InterpolationType::bounce)
			result = glm::bounceEaseIn(t);
	}
	else if (ease == Ease::out) {

	}
	else if (ease == Ease::inOut) {

	}
	else if (ease == Ease::Outin) {

	}

	return result;
}
*/
