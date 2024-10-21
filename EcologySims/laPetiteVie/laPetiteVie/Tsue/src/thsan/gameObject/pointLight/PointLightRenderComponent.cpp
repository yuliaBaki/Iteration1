#include "PointLightRenderComponent.h"

PointLightRenderComponent::PointLightRenderComponent(sf::Vector3f position, float constant, float linear, float quadratic, sf::Color ambient, sf::Color diffuse, sf::Color specular)
{
	lightData = new LightData();
	auto& lp = lightData->lightProperties;
	lp.position = position;
	lp.attenuationConstant = constant;
	lp.attenuationLinear = linear;
	lp.attenuationQuadratic = quadratic;
	lp.ambient = ambient;
	lp.diffuse = diffuse;
	lp.specular = specular;

	isLoaded = false;
}

PointLightRenderComponent::PointLightRenderComponent(PointLight* pl):
	pl(pl)
{

	lightData = new LightData();
	auto& lp = lightData->lightProperties;
	auto lp_pl = pl->getLightProperties();
	lp.position = lp_pl.position;
	lp.attenuationConstant = lp_pl.attenuationConstant;
	lp.attenuationLinear = lp_pl.attenuationLinear;
	lp.attenuationQuadratic = lp_pl.attenuationQuadratic;
	lp.ambient = lp_pl.ambient;
	lp.diffuse = lp_pl.diffuse;
	lp.specular = lp_pl.specular;

	isLoaded = true;
}

void PointLightRenderComponent::init( Scene& scene)
{
	parent->setComponent(COMPONENT_DATA::LIGHT, lightData); //no need to check, since from the constructor we expect a new light source

	auto& lp = lightData->lightProperties;
	if(!isLoaded)
		pl = scene.createPointLight(lp.position, lp.attenuationConstant, lp.attenuationLinear, lp.attenuationQuadratic, lp.ambient, lp.diffuse, lp.specular);
	lightData->lightProperties = pl->getLightProperties();
}

void PointLightRenderComponent::update(Scene& scene, const sf::Time& dt, sf::RenderTarget& target, RendererPseudo3D& rendererPseudo3D)
{
	auto& lp = lightData->lightProperties;

	pl->setAmbientColor(lp.ambient);
	pl->setDiffuseColor(lp.diffuse);
	pl->setSpecularColor(lp.specular);

	pl->setPosition(lp.position);
	pl->setAttenuation(lp.attenuationConstant, lp.attenuationLinear, lp.attenuationQuadratic);

	lightData->lightProperties = pl->getLightProperties();
}

void PointLightRenderComponent::onDelete(Scene& scene)
{
	scene.deletePointLight(pl);
}

PointLightRenderComponent::~PointLightRenderComponent()
{
	
}
