#include "helperMath.h"
#include <math.h>

glm::vec3 SmoothDamp(glm::vec3 current, glm::vec3 target, glm::vec3& currentVelocity, float smoothTime, float maxSpeed, float deltaTime)
{
	return SmoothDamp(current, target, currentVelocity, smoothTime, maxSpeed, deltaTime);
}

 float SmoothDamp(float current, float target, float& currentVelocity, float smoothTime, float deltaTime)
{
    float maxSpeed = std::numeric_limits<float>::infinity();
    return SmoothDamp(current, target, currentVelocity, smoothTime, maxSpeed, deltaTime);
}

// Gradually changes a value towards a desired goal over time.
float SmoothDamp(float current, float target, float& currentVelocity, float smoothTime, float maxSpeed, float deltaTime)
{
    // Based on Game Programming Gems 4 Chapter 1.10
    smoothTime = glm::max(0.0001f, smoothTime);
    float omega = 2.0f / smoothTime;

    float x = omega * deltaTime;
    float exp = 1.0f / (1.0f + x + 0.48F * x * x + 0.235F * x * x * x);
    float change = current - target;
    float originalTo = target;

    // Clamp maximum speed
    float maxChange = maxSpeed * smoothTime;
    change = glm::clamp(change, -maxChange, maxChange);
    target = current - change;

    float temp = (currentVelocity + omega * change) * deltaTime;
    currentVelocity = (currentVelocity - omega * temp) * exp;
    float output = target + (change + temp) * exp;

    // Prevent overshooting
    if (originalTo - current > 0.0F == output > originalTo)
    {
        output = originalTo;
        currentVelocity = (output - originalTo) / deltaTime;
    }

    return output;
}