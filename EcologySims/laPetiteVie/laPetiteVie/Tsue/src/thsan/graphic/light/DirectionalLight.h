#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "Light.h"

class DirectionalLight
{
    public:
        DirectionalLight(sf::Vector3f direction = sf::Vector3f(0.0,1.0,0.0));
        void enable(bool isEnable);

        void setAmbientColor(sf::Color color);
        void setDiffuseColor(sf::Color color);
        void setSpecularColor(sf::Color color);

        void setDirection(sf::Vector3f direction);

        LightProperties getLightProperties();
    private:
        LightProperties lightProperties;
};

#endif // DIRECTIONALLIGHT_H
