#include "SpriteAnimation.h"
#include "../ressourceManager/TextureManager.h"
#include <glm/glm.hpp>
#include <memory.h>
#include <iostream>

using namespace std;

SpriteAnimation::SpriteAnimation():
    depthEnable(false),
    emissionEnable(false),
    mainAnimation(""),
    currAnimation(mainAnimation),
    xFlip(1),
    yFlip(1),
    hidden(false),
    shininess(32),
    depthFactor(10)
{
}

SpriteAnimation::SpriteAnimation(const std::string& SpriteSheetLocation)
{
    init(SpriteSheetLocation);
}

SpriteAnimation::SpriteAnimation(const sf::Texture& texture)
{
    init(texture);
}

void SpriteAnimation::init(const std::string& SpriteSheetLocation)
{
	std::shared_ptr<sf::Texture> tex = RessourceManager::TextureManager::get(SpriteSheetLocation);
	setTexture(*tex);
	spriteSheetSize = tex->getSize();

    depthEnable = false;
    emissionEnable = false;
	mainAnimation = "";
	currAnimation = mainAnimation;
	xFlip = 1;
	yFlip = 1;
    hidden = false;
}

void SpriteAnimation::init(const sf::Texture& texture)
{
    setTexture(texture);
    spriteSheetSize = texture.getSize();

    mainAnimation = "";
    currAnimation = mainAnimation;
    xFlip = 1;
    yFlip = 1;
    hidden = false;
}

void SpriteAnimation::flipX()
{
	xFlip *= -1;
}

void SpriteAnimation::flipY()
{
	yFlip *= -1;

}

bool SpriteAnimation::isFlipX()
{
	if(xFlip == 1)
		return false;
	return true;
}

bool SpriteAnimation::isFlipY()
{
	if (yFlip == 1)
		return false;
	return true;
}

bool SpriteAnimation::hasAnimation(const std::string& name)
{
    if (animations.find(name) != animations.end())
        return true;
    return false;
}


void SpriteAnimation::setCurrAnimation(std::string name)
{
    if (currAnimation != name) {
        tempsAccumuleAnimation = sf::Time::Zero;
        animation_is_done = false;
    }
    currAnimation = name;
}

std::string SpriteAnimation::getCurrAnimation()
{
    return currAnimation;
}

unsigned int SpriteAnimation::getCurrAnimationFrameIndex()
{
    return animations[currAnimation].getCurrIndex();
}

void SpriteAnimation::enableLoop(bool enable)
{
    animations[currAnimation].loopEnable(enable);
}

void SpriteAnimation::resetCurrAnimation()
{
    animations[currAnimation].reset();
    tempsAccumuleAnimation = sf::Time::Zero;
}

void SpriteAnimation::setMainAnimation(std::string name)
{
    mainAnimation = name;
}

std::string SpriteAnimation::getMainAnimation()
{
    return mainAnimation;
}

void SpriteAnimation::addFrame(std::string name, sf::IntRect frameRect, sf::Time frameTime)
{
    if (animations.find(name) == animations.end())
        animations.emplace(std::make_pair(name, AnimationData{}));

    animations[name].addFrame(frameRect, frameTime);
}


sf::IntRect SpriteAnimation::getCurrFrameRect()
{
    return animations[currAnimation].getCurrentFrameRect();
}

void SpriteAnimation::update(const sf::Time& deltaTime)
{
    if(animations[currAnimation].getSize() == 0)
        return;
    else{
        if (tempsAccumuleFrame > animations[currAnimation].getCurrentFrameTime()){
            animations[currAnimation].nextFrame();
            tempsAccumuleFrame = sf::Time::Zero;
        }

        if (tempsAccumuleAnimation > animations[currAnimation].getTotalDuration()) {
            tempsAccumuleAnimation = sf::Time::Zero;
            if(!animations[currAnimation].isLoop())
                animation_is_done = true;
        }

        tempsAccumuleFrame += deltaTime;

        if (!animation_is_done) {
            tempsAccumuleAnimation += deltaTime;
        }
    }

    makeDrawable();
}

void SpriteAnimation::makeDrawable()
{
    setTextureRect(animations[currAnimation].getCurrentFrameRect());
    setOrigin(getLocalBounds().left + getLocalBounds().width/2,
                       getLocalBounds().top  + getLocalBounds().height/2);
	setScale(xFlip, yFlip);
}

void SpriteAnimation::hide()
{
    hidden = true;
}

void SpriteAnimation::show()
{
    hidden = false;
}

bool SpriteAnimation::isHidden()
{
    return hidden;
}



sf::Vector2u SpriteAnimation::getSpriteSheetSize()
{
	return spriteSheetSize;
}

bool SpriteAnimation::hasDepthTexture()
{
    return depthEnable;
}

void SpriteAnimation::setDepthTexture(const std::string& path)
{
    depthTexture = RessourceManager::TextureManager::get(path);
    depthTexturePath = path;

    if (depthTexture)
        depthEnable = true;
    else
        depthEnable = false;
}

void SpriteAnimation::setKeyColor(sf::Color color)
{
    keyColor = color;
}

sf::Color SpriteAnimation::getKeyColor()
{
    return keyColor;
}

void SpriteAnimation::setShininess(int shininess)
{
    this->shininess = shininess;
}

int SpriteAnimation::getShininess()
{
    return shininess;
}

bool SpriteAnimation::hasEmissionTexture()
{
    return emissionEnable;
}

void SpriteAnimation::setEmissionTexture(const std::string& path)
{
    emissionTexture = RessourceManager::TextureManager::get(path);
    emissionTexturePath = path;
    if (emissionTexture)
        emissionEnable = true;
    else
        emissionEnable = false;
}

std::shared_ptr<sf::Texture> SpriteAnimation::GetDepthTexture() const{
	return depthTexture;
}

std::string SpriteAnimation::getDepthTexturePath()
{
    return depthTexturePath;
}

std::shared_ptr<sf::Texture> SpriteAnimation::getEmissionTexture() const {
    return emissionTexture;
}

std::string SpriteAnimation::getEmissionTexturePath()
{
    return emissionTexturePath;
}

float SpriteAnimation::getDepthFactor()
{
    return depthFactor;
}

void SpriteAnimation::setDepthFactor(float depthFactor)
{
    if (depthFactor > 0)
        this->depthFactor = depthFactor;
}

sf::Time SpriteAnimation::getCurrTimeDuration()
{
    return animations[currAnimation].getTotalDuration();
}

sf::Time SpriteAnimation::getCurrTimeReached()
{
    return tempsAccumuleAnimation;
}
