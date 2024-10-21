#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Light.h"

class PointLight
{
    public:
        PointLight(sf::Vector3f position = sf::Vector3f());
        void enable(bool isEnable);

        void setAmbientColor(sf::Color color);
        void setDiffuseColor(sf::Color color);
        void setSpecularColor(sf::Color color);

        void setAttenuation(float constant, float linear, float quadratic);
        void setPosition(sf::Vector3f position);

        LightProperties getLightProperties();

    private:
        LightProperties lightProperties;

};

#endif // POINTLIGHT_H
