#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include <vector>

class Animal {
private:
    std::shared_ptr<std::vector<sf::Texture>> gifFrames;
    sf::Vector2f targetPos;
    sf::Clock frameClock; // To control the frame switching time
    sf::Clock hungerClock; // To reduce hunger over time
    sf::Clock clock;
    sf::Sprite objectSprite;
    bool isFlipped; // To check if the object is currently moving
    float life; // Maximum life is 100
    float hunger; // Hunger starts at 100 and reduces over time
    int currentFrame;
    sf::RectangleShape hungerBar;
    sf::RectangleShape lifeBar;
    sf::Vector2f moveDirection;   // Move right initially
    bool moving;

public:
    // Constructor
    Animal(std::shared_ptr<std::vector<sf::Texture>> gifFrames_, const sf::Vector2f& targetPos_,
        const sf::Sprite& objectSprite_, float life_, float hunger_, bool isFlipped_, const sf::Vector2f& moveDirection_, bool moving_,
        const sf::RectangleShape& hungerBar_, const sf::RectangleShape& lifeBar_)
        : gifFrames(gifFrames_), targetPos(targetPos_), objectSprite(objectSprite_),
        life(life_), hunger(hunger_), isFlipped(isFlipped_), moveDirection(moveDirection_), moving(moving_), hungerBar(hungerBar_), lifeBar(lifeBar_) {

        currentFrame = 0;

    }

    // Copy Constructor
    Animal(const Animal& other)
        : gifFrames(other.gifFrames), targetPos(other.targetPos), frameClock(other.frameClock),
        hungerClock(other.hungerClock),clock(other.clock), objectSprite(other.objectSprite), isFlipped(other.isFlipped),
        life(other.life), hunger(other.hunger), hungerBar(other.hungerBar),currentFrame(other.currentFrame), lifeBar(other.lifeBar), moveDirection(other.moveDirection), moving(other.moving) {}

    // Destructor
    ~Animal() = default;

    // Getters
    std::shared_ptr<std::vector<sf::Texture>> getGifFrames() const { return gifFrames; }
    sf::Vector2f getCurrentPos() const { return objectSprite.getPosition(); }
    sf::Vector2f getTargetPos() const { return targetPos; }
    sf::Vector2f getMoveDirection() const { return moveDirection; }
    sf::Clock& getFrameClock() { return frameClock; } 
    sf::Clock& getHungerClock() { return hungerClock; }
    sf::Clock& getClock() { return clock; }
    sf::Sprite& getObjectSprite() { return objectSprite; }
    bool getIsFlipped() const { return isFlipped; }
    bool getisMoving() const { return moving; }
    float getLife() const { return life; }
    float getHunger() const { return hunger; }
    sf::RectangleShape& getHungerBar() { return hungerBar; }
    sf::RectangleShape& getLifeBar() { return lifeBar; }

    // Setters
    void setHunger(float newHunger) { hunger = newHunger; hungerBar.setSize(sf::Vector2f(hunger, 10)); }
    void setLife(float newLife) { life = newLife; lifeBar.setSize(sf::Vector2f(life, 10)); }


    // methods
    void updateSpriteTexture() {
        currentFrame = (currentFrame + 1) % gifFrames->size();  // Loop through frames
        objectSprite.setTexture((*gifFrames)[currentFrame]);      // Update the sprite texture
        frameClock.restart();
    }

    void updateHunger(float hungerDecayRate, float hungerThreshold) {
        hunger -= hungerDecayRate;  // Reduce hunger over time
        if (hunger < hungerThreshold) {
            life -= 1.0f;  // Reduce life if hunger is zero
        }
        hungerClock.restart();  // Reset the hunger clock
    }

    void setNewTargetPosition(sf::Vector2f newPosition) {
        targetPos = newPosition;  // Change the target to the first detected object
        moveDirection = getDirectionToTarget(objectSprite.getPosition(), targetPos);
        moving = true;  // Ensure the object keeps moving towards the new target
    }

    // returns true if the targetPosition is reached
    bool moveByOneStep(float speed) {
        if (moving) {
            sf::Vector2f currentPos = objectSprite.getPosition();

            // Check if we need to flip the sprite based on movement direction
            if (moveDirection.x > 0 && isFlipped) {
                objectSprite.setScale(0.3f, 0.3f);  // Unflip when moving right
                isFlipped = false;
            }
            else if (moveDirection.x < 0 && !isFlipped) {
                objectSprite.setScale(-0.3f, 0.3f);  // Flip when moving left
                isFlipped = true;
            }

            // Move the object in the direction of the target
            objectSprite.move(moveDirection.x * speed, moveDirection.y * speed);

            // If the object is close enough to the target, stop moving
            if (std::abs(targetPos.x - currentPos.x) < 1.f && std::abs(targetPos.y - currentPos.y) < 1.f) {
                objectSprite.setPosition(targetPos);  // Snap to the exact position
                moving = false;  // Stop moving

                return true;
            }
        }
        return false;
    }

    void updateHungerBar() {
        hungerBar.setSize(sf::Vector2f(hunger, 10));
    }

    void updateLifeBar() {
        lifeBar.setSize(sf::Vector2f(life, 10));
    }

    // Function to get the normalized direction from the current position to the target
    static sf::Vector2f getDirectionToTarget(const sf::Vector2f& currentPos, const sf::Vector2f& targetPos) {
        sf::Vector2f direction = targetPos - currentPos;

        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length != 0) {
            direction /= length;  // Normalize the direction vector
        }

        return direction;
    }
};

