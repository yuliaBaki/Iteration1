#include "PointLight.h"

PointLight::PointLight(sf::Vector3f position)
{
    lightProperties.isEnable = true;
    lightProperties.isPoint = true;
    lightProperties.isDirection = false;
    lightProperties.isSpot = false;
    lightProperties.isCursor = false;

    lightProperties.position = position;
    lightProperties.attenuationConstant = 1.0;
    lightProperties.attenuationLinear = 0.0;
    lightProperties.attenuationQuadratic = 0.0;
}

void PointLight::enable(bool isEnable)
{
    lightProperties.isEnable = isEnable;
}

void PointLight::setAmbientColor(sf::Color color)
{
    lightProperties.ambient = color;
}

void PointLight::setDiffuseColor(sf::Color color)
{
    lightProperties.diffuse = color;
}

void PointLight::setSpecularColor(sf::Color color)
{
    lightProperties.specular = color;
}

void PointLight::setAttenuation(float constant, float linear, float quadratic)
{
    lightProperties.attenuationConstant = constant;
    lightProperties.attenuationLinear = linear;
    lightProperties.attenuationQuadratic = quadratic;
}

void PointLight::setPosition(sf::Vector3f position)
{
    lightProperties.position = position;
}

LightProperties PointLight::getLightProperties()
{
    return lightProperties;
}
