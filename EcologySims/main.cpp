#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/System.hpp>
#include <iostream>
#include <random>
#include <ctime>
#include <cmath>
#include <vector>
#include "Animal.h"
#include "Prey.h"
#include "Predator.h"


// Load all frames of the "GIF"
std::vector<sf::Texture> loadPreyGifFrames() {
    std::vector<sf::Texture> frames;

    for (int i = 1; i <= 8; ++i) {
        sf::Texture frame;
        if (frame.loadFromFile("Images/lapin/" + std::to_string(i) + ".png")) {
            frames.push_back(frame);
        }
        else {
            std::cerr << "Error: Could not load frame " << i << std::endl;
        }
    }
    return frames;
}

std::vector<sf::Texture> loadPredatorGifFrames() {
    std::vector<sf::Texture> frames;

    for (int i = 1; i <= 18; ++i) {
        sf::Texture frame;
        if (frame.loadFromFile("Images/brun/" + std::to_string(i) + ".png")) {
            frames.push_back(frame);
        }
        else {
            std::cerr << "Error: Could not load frame " << i << std::endl;
        }
    }
    return frames;
}

// Function to generate a random position within the window, ensuring it's on a green area
sf::Vector2f getRandomGreenPosition(const sf::Image& backgroundImage, const sf::RenderWindow& window) {
    sf::Vector2u windowSize = window.getSize();

    static std::mt19937 rng(static_cast<unsigned int>(std::time(0))); // Random number generator
    std::uniform_int_distribution<int> xDist(0, windowSize.x - 1);
    std::uniform_int_distribution<int> yDist(0, windowSize.y - 1);

    sf::Vector2f position;
    bool foundGreenArea = false;
    while (!foundGreenArea) {
        int x = xDist(rng);
        int y = yDist(rng);

        sf::Color pixelColor = backgroundImage.getPixel(x, y);
        if (pixelColor == sf::Color(0, 255, 0)) {
            position = sf::Vector2f(static_cast<float>(x), static_cast<float>(y));
            foundGreenArea = true;
        }
    }

    return position;
}


// Function to detect nearby objects within a certain radius
std::vector<sf::Vector2f> detectObjectsAround(const sf::Vector2f& objectPosition, const std::vector<sf::Vector2f>& otherObjects, float detectionRadius) {
    std::vector<sf::Vector2f> detectedObjects;

    for (const sf::Vector2f& otherPosition : otherObjects) {
        float distance = std::sqrt(std::pow(otherPosition.x - objectPosition.x, 2) + std::pow(otherPosition.y - objectPosition.y, 2));

        if (distance <= detectionRadius) {
            detectedObjects.push_back(otherPosition);  // Add the detected object to the list
        }
    }

    return detectedObjects;
}

sf::RectangleShape createHungerBar(float positionX, float positionY, float hunger) {
    sf::RectangleShape hungerBar(sf::Vector2f(hunger, 10));  // Create a bar for hunger
    hungerBar.setFillColor(sf::Color::Yellow);  // Set color for the hunger bar
    hungerBar.setPosition(positionX, positionY);
    return hungerBar;
}

sf::RectangleShape createLifeBar(float positionX, float positionY, float life) {
    sf::RectangleShape lifeBar(sf::Vector2f(life, 10));  // Create a bar for life
    lifeBar.setFillColor(sf::Color::Red);  // Set color for the life bar
    lifeBar.setPosition(positionX, positionY);  // Position the life bar
    return lifeBar;
}

Prey createPrey(std::shared_ptr<std::vector<sf::Texture>> sharedGigFrames,
    const sf::Image& backgroundImage,
    const sf::RenderWindow& window
) {
    std::string gender = (std::rand() % 2 == 0) ? "Male" : "Female";
    // Create the sprite for the animated object
    sf::Sprite objectSprite;
    objectSprite.setScale(0.3f, 0.3f);  // Scale down the object
    objectSprite.setTexture((*sharedGigFrames)[0]);  // Set the initial frame

    // Set an initial random position for the object in green areas
    sf::Vector2f targetPosition = getRandomGreenPosition(backgroundImage, window);
    objectSprite.setPosition(targetPosition);  // Start at an initial target

    sf::Vector2f moveDirection(1.f, 0.f);   // Move right initially
    bool moving = true;
    bool isFlipped = false;  // To check if the object is currently moving

    // Life and Hunger system
    float life = 100.0f;  // Maximum life is 100
    float hunger = 100.0f;  // Hunger starts at 100 and reduces over time

    return Prey(
        sharedGigFrames,
        targetPosition,
        objectSprite,
        life,
        hunger,
        isFlipped,
        moveDirection,
        moving,
        createHungerBar(50, 50, hunger),
        createLifeBar(50, 70, life),
        gender
    );
}

void handlePreyReproduction(std::vector<Prey>& preys, std::shared_ptr<std::vector<sf::Texture>>& preyGifFrames, const sf::Image& backgroundImage, sf::RenderWindow& window) {
    // Implement logic to check for reproduction conditions
    for (size_t i = 0; i < preys.size(); ++i) {
        for (size_t j = i + 1; j < preys.size(); ++j) {
            if (preys[i].canReproduceWith(preys[j])) {  // Check if they can reproduce
                
                std::cerr << preys[i].canReproduceWith(preys[j]) << std::endl;
                
                // Create a new prey and add it to the vector
                Prey newPrey = createPrey(preyGifFrames, backgroundImage, window);
                preys.push_back(newPrey);
                std::cout << "New prey created!" << std::endl;
                // You might want to set a flag or decrement their energy if needed
                preys[i].resetReproduction();  // Reset or update the parents if needed
                preys[j].resetReproduction();  // Reset or update the parents if needed
            }
        }
    }
}

Predator createPredator(std::shared_ptr<std::vector<sf::Texture>> sharedGigFrames,
    const sf::Image& backgroundImage,
    const sf::RenderWindow& window
) {
    std::string gender = (std::rand() % 2 == 0) ? "Male" : "Female";
    // Create the sprite for the animated object
    sf::Sprite objectSprite;
    objectSprite.setScale(0.3f, 0.3f);  // Scale down the object
    objectSprite.setTexture((*sharedGigFrames)[0]);  // Set the initial frame

    // Set an initial random position for the object in green areas
    sf::Vector2f targetPosition = getRandomGreenPosition(backgroundImage, window);
    objectSprite.setPosition(targetPosition);  // Start at an initial target

    sf::Vector2f moveDirection(1.f, 0.f);   // Move right initially
    bool moving = true;
    bool isFlipped = false;  // To check if the object is currently moving

    // Life and Hunger system
    float life = 100.0f;  // Maximum life is 100
    float hunger = 100.0f;  // Hunger starts at 100 and reduces over time

    return Predator(
        sharedGigFrames,
        targetPosition,
        objectSprite,
        life,
        hunger,
        isFlipped,
        moveDirection,
        moving,
        createHungerBar(50, 50, hunger),
        createLifeBar(50, 70, life),
        gender
    );
}

void handlePredatorReproduction(std::vector<Predator>& predators, std::shared_ptr<std::vector<sf::Texture>>& predatorGifFrames, const sf::Image& backgroundImage, sf::RenderWindow& window) {
    // Implement logic to check for reproduction conditions
    for (size_t i = 0; i < predators.size(); ++i) {
        for (size_t j = i + 1; j < predators.size(); ++j) {
            if (predators[i].canReproduceWith(predators[j])) {  // Check if they can reproduce
                // Create a new predator and add it to the vector
                Predator newPredator = createPredator(predatorGifFrames, backgroundImage, window);
                predators.push_back(newPredator);
                std::cout << "New predator created!" << std::endl;
                // You might want to set a flag or decrement their energy if needed
                predators[i].resetReproduction();  // Reset or update the parents if needed
                predators[j].resetReproduction();  // Reset or update the parents if needed
            }
        }
    }
}



int main() {
    float speed = 0.05f;
    float detectionRadius = 100.f;  // Define the radar detection radius


    sf::RenderWindow window(sf::VideoMode(1440, 960), "The Game!");

    sf::Texture backgroundTexture;
    sf::Image backgroundImage;
    if (!backgroundTexture.loadFromFile("Images/BG.png") || !backgroundImage.loadFromFile("Images/BG.png")) {
        std::cerr << "Error: Could not load the background image!" << std::endl;
        return -1;
    }

    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    std::vector<sf::Texture> preyGifFrames = loadPreyGifFrames();
    if (preyGifFrames.empty()) {
        std::cerr << "Error: No Prey GIF frames loaded!" << std::endl;
        return -1;
    }
    std::vector<sf::Texture> predatorGifFrames = loadPredatorGifFrames();
    if (predatorGifFrames.empty()) {
        std::cerr << "Error: No Predator GIF frames loaded!" << std::endl;
        return -1;
    }
    sf::Clock spawnClock;
    sf::Clock reproductionClock;  // Clock to control reproduction rate
    float frameSwitchTime = 0.1f;  // Time between each frame in seconds
    float hungerDecayRate = 0.5f;  // Hunger decreases by 0.5 every second
    float hungerThreshold = 0.0f;  // Once hunger reaches 0, life decreases
    float reproductionInterval = 10.0f;  // Time interval for checking reproduction

    // Example list of other objects (positions) in the game
    std::vector<sf::Vector2f> otherObjects;

    auto preySharedGigFrames = std::make_shared<std::vector<sf::Texture>>(preyGifFrames);

    std::vector<Prey> preys;
    for (int i = 0; i < 3; ++i) {
        Prey animal = createPrey(preySharedGigFrames, backgroundImage, window);
        preys.push_back(animal);
        std::cout << "Prey gender: " << animal.getGender() << std::endl;
    }

    auto predatorSharedGigFrames = std::make_shared<std::vector<sf::Texture>>(predatorGifFrames);
    std::vector<Predator> predators;
    for (int i = 0; i < 1; ++i) {
        Predator animal = createPredator(predatorSharedGigFrames, backgroundImage, window);
        predators.push_back(animal);
        std::cout << "Predator gender: " << animal.getGender() << std::endl;
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Generate a new object every 30 seconds
        if (spawnClock.getElapsedTime().asSeconds() >= 3.0f) {
            sf::Vector2f newObjectPosition = getRandomGreenPosition(backgroundImage, window);
            otherObjects.push_back(newObjectPosition);  // Add new object
            spawnClock.restart();
        }

        // Handle reproduction every 10 seconds
        if (reproductionClock.getElapsedTime().asSeconds() >= reproductionInterval) {
            // Handle prey reproduction
            handlePreyReproduction(preys, preySharedGigFrames, backgroundImage, window);

            // Handle predator reproduction
            handlePredatorReproduction(predators, predatorSharedGigFrames, backgroundImage, window);

            // Reset the reproduction clock
            reproductionClock.restart();
        }

        window.clear();
        window.draw(backgroundSprite);

        for (Animal& prey : preys) {
            if (prey.getFrameClock().getElapsedTime().asSeconds() >= frameSwitchTime) {
                prey.updateSpriteTexture();
            }
            sf::Vector2f currentPos = prey.getObjectSprite().getPosition();
            std::vector<sf::Vector2f> detectedObjects = detectObjectsAround(currentPos, otherObjects, detectionRadius);
            sf::CircleShape detectionMarker(10);  // Circle with a radius of 5
            detectionMarker.setFillColor(sf::Color::Red);
            for (const auto& detectedPos : detectedObjects) {
                detectionMarker.setPosition(detectedPos.x - detectionMarker.getRadius(), detectedPos.y - detectionMarker.getRadius());
                window.draw(detectionMarker);
            }
            // Hunger decay logic
            if (prey.getHungerClock().getElapsedTime().asSeconds() >= 1.0f) {
                prey.updateHunger(hungerDecayRate, hungerThreshold);
            }
            // If detected objects exist, set the first one as the new target
            if (!detectedObjects.empty()) {
                prey.setNewTargetPosition(detectedObjects[0]);
            }
            // Check if it's time to generate a new target position
            if (prey.getClock().getElapsedTime().asSeconds() >= 2.0f && !prey.getisMoving() && detectedObjects.empty()) {
                prey.setNewTargetPosition(getRandomGreenPosition(backgroundImage, window));
                prey.getClock().restart();
            }
            bool targetReached = prey.moveByOneStep(speed);
            if (targetReached) {
                // Remove the detected object from the list once reached
                auto it = std::find(otherObjects.begin(), otherObjects.end(), prey.getTargetPos());
                if (it != otherObjects.end()) {
                    otherObjects.erase(it);  // Remove reached object from the list
                    prey.setHunger(100.0f);
                }
            }

            window.draw(prey.getObjectSprite());
            prey.updateHungerBar();
            prey.updateLifeBar();
            window.draw(prey.getHungerBar());  // Draw the hunger bar
            window.draw(prey.getLifeBar());  // Draw the life bar
        }

        for (Animal& predator : predators) {
            if (predator.getFrameClock().getElapsedTime().asSeconds() >= frameSwitchTime) {
                predator.updateSpriteTexture();
            }
            sf::Vector2f currentPos = predator.getObjectSprite().getPosition();
            std::vector<Prey*> detectedPrey;

            for (Prey& prey : preys) {
                float distance = std::sqrt(std::pow(prey.getObjectSprite().getPosition().x - currentPos.x, 2) +
                    std::pow(prey.getObjectSprite().getPosition().y - currentPos.y, 2));

                if (distance <= detectionRadius) {
                    detectedPrey.push_back(&prey);
                }
            }

            // Hunger decay logic
            if (predator.getHungerClock().getElapsedTime().asSeconds() >= 1.0f) {
                predator.updateHunger(hungerDecayRate, hungerThreshold);
            }

            // If detected prey exists, set the first one as the new target
            if (!detectedPrey.empty()) {
                predator.setNewTargetPosition(detectedPrey[0]->getObjectSprite().getPosition());
            }

            // Check if it's time to generate a new target position if no prey is nearby
            if (predator.getClock().getElapsedTime().asSeconds() >= 2.0f && !predator.getisMoving() && detectedPrey.empty()) {
                predator.setNewTargetPosition(getRandomGreenPosition(backgroundImage, window));
                predator.getClock().restart();
            }
            bool targetReached = predator.moveByOneStep(speed);
            if (targetReached && !detectedPrey.empty()) {
                // Find the prey that was reached and remove it
              
                Prey* preyToKill = detectedPrey[0];
                auto it = std::remove_if(preys.begin(), preys.end(),
                    [preyToKill](Prey& prey) { return &prey == preyToKill; });
                preys.erase(it, preys.end());  // Remove prey from vector

                // Reset predator hunger
                predator.setHunger(100.0f);
            }

            // Draw the predator and its hunger/life bars
            window.draw(predator.getObjectSprite());
            predator.updateHungerBar();
            predator.updateLifeBar();
            window.draw(predator.getHungerBar());
            window.draw(predator.getLifeBar());
        }

        // Loop through the remaining prey and draw them
        for (Prey& prey : preys) {
            if (prey.getFrameClock().getElapsedTime().asSeconds() >= frameSwitchTime) {
                prey.updateSpriteTexture();
            }

            sf::Vector2f currentPos = prey.getObjectSprite().getPosition();
            bool targetReached = prey.moveByOneStep(speed);

            // Draw the prey and its hunger/life bars
            window.draw(prey.getObjectSprite());
            prey.updateHungerBar();
            prey.updateLifeBar();
            window.draw(prey.getHungerBar());
            window.draw(prey.getLifeBar());
        }
        

        window.display();


    }

    return 0;
}