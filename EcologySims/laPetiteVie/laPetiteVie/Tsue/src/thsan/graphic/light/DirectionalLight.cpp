#include "DirectionalLight.h"


DirectionalLight::DirectionalLight(sf::Vector3f direction)
{
    lightProperties.isEnable = true;
    lightProperties.isPoint = false;
    lightProperties.isDirection = true;
    lightProperties.isSpot = false;
    lightProperties.isCursor = false;

    lightProperties.direction = direction;
}

void DirectionalLight::enable(bool isEnable)
{
    lightProperties.isEnable = isEnable;
}

void DirectionalLight::setAmbientColor(sf::Color color)
{
    lightProperties.ambient = color;

}

void DirectionalLight::setDiffuseColor(sf::Color color)
{
    lightProperties.diffuse = color;
}

void DirectionalLight::setSpecularColor(sf::Color color)
{
    lightProperties.specular = color;
}

void DirectionalLight::setDirection(sf::Vector3f direction)
{
    lightProperties.direction = direction;
}

LightProperties DirectionalLight::getLightProperties()
{
    return lightProperties;
}
