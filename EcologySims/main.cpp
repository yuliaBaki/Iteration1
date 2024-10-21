#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/System.hpp>
#include <iostream>
#include <random>
#include <ctime>
#include <cmath>
#include <vector>


// Load all frames of the "GIF"
std::vector<sf::Texture> loadGifFrames() {
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

// Function to get the normalized direction from the current position to the target
sf::Vector2f getDirectionToTarget(const sf::Vector2f& currentPos, const sf::Vector2f& targetPos) {
    sf::Vector2f direction = targetPos - currentPos;

    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0) {
        direction /= length;  // Normalize the direction vector
    }

    return direction;
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

    std::vector<sf::Texture> gifFrames = loadGifFrames();
    if (gifFrames.empty()) {
        std::cerr << "Error: No GIF frames loaded!" << std::endl;
        return -1;
    }

    sf::Clock frameClock;  // To control the frame switching time
    sf::Clock hungerClock; // To reduce hunger over time
    sf::Clock spawnClock;
    float frameSwitchTime = 0.1f;  // Time between each frame in seconds
    int currentFrame = 0;  // Keep track of which frame is currently displayed

    // Create the sprite for the animated object
    sf::Sprite objectSprite;
    objectSprite.setScale(0.3f, 0.3f);  // Scale down the object
    objectSprite.setTexture(gifFrames[0]);  // Set the initial frame

    // Set an initial random position for the object in green areas
    sf::Vector2f targetPosition = getRandomGreenPosition(backgroundImage, window);
    objectSprite.setPosition(targetPosition);  // Start at an initial target

    sf::Clock clock;
    sf::Vector2f moveDirection(1.f, 0.f);   // Move right initially
    bool moving = true;
    bool isFlipped = false;  // To check if the object is currently moving

    // Life and Hunger system
    float life = 100.0f;  // Maximum life is 100
    float hunger = 100.0f;  // Hunger starts at 100 and reduces over time
    float hungerDecayRate = 0.5f;  // Hunger decreases by 0.5 every second
    float hungerThreshold = 0.0f;  // Once hunger reaches 0, life decreases

    // Example list of other objects (positions) in the game
    std::vector<sf::Vector2f> otherObjects;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (frameClock.getElapsedTime().asSeconds() >= frameSwitchTime) {
            currentFrame = (currentFrame + 1) % gifFrames.size();  // Loop through frames
            objectSprite.setTexture(gifFrames[currentFrame]);      // Update the sprite texture
            frameClock.restart();
        }

        sf::Vector2f currentPos = objectSprite.getPosition();
        std::vector<sf::Vector2f> detectedObjects = detectObjectsAround(currentPos, otherObjects, detectionRadius);

        // Hunger decay logic
        if (hungerClock.getElapsedTime().asSeconds() >= 1.0f) {
            hunger -= hungerDecayRate;  // Reduce hunger over time
            if (hunger < hungerThreshold) {
                life -= 1.0f;  // Reduce life if hunger is zero
            }
            hungerClock.restart();  // Reset the hunger clock
        }

        // Generate a new object every 30 seconds
        if (spawnClock.getElapsedTime().asSeconds() >= 30.0f) {
            sf::Vector2f newObjectPosition = getRandomGreenPosition(backgroundImage, window);
            otherObjects.push_back(newObjectPosition);  // Add new object
            spawnClock.restart();
        }

        // If detected objects exist, set the first one as the new target
        if (!detectedObjects.empty()) {
            targetPosition = detectedObjects[0];  // Change the target to the first detected object
            moveDirection = getDirectionToTarget(currentPos, targetPosition);
            moving = true;  // Ensure the object keeps moving towards the new target
        }
        // Check if it's time to generate a new target position
        if (clock.getElapsedTime().asSeconds() >= 2.0f && !moving && detectedObjects.empty()) {
            targetPosition = getRandomGreenPosition(backgroundImage, window);
            moveDirection = getDirectionToTarget(objectSprite.getPosition(), targetPosition);
            moving = true;  // The object will now start moving towards the target
            clock.restart(); // Reset the clock
        }

        // Move the object step by step towards the target position
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
            if (std::abs(targetPosition.x - currentPos.x) < 1.f && std::abs(targetPosition.y - currentPos.y) < 1.f) {
                objectSprite.setPosition(targetPosition);  // Snap to the exact position
                moving = false;  // Stop moving


                // Remove the detected object from the list once reached
                auto it = std::find(otherObjects.begin(), otherObjects.end(), targetPosition);
                if (it != otherObjects.end()) {
                    otherObjects.erase(it);  // Remove reached object from the list
                    hunger = 100.0f;
                }
            }
        }

        window.clear();
        window.draw(backgroundSprite);
        window.draw(objectSprite);

        // Optional: Display Life and Hunger as bars or text
        sf::RectangleShape hungerBar(sf::Vector2f(hunger, 10));  // Create a bar for hunger
        hungerBar.setFillColor(sf::Color::Yellow);  // Set color for the hunger bar
        hungerBar.setPosition(50, 50);  // Position the hunger bar

        sf::RectangleShape lifeBar(sf::Vector2f(life, 10));  // Create a bar for life
        lifeBar.setFillColor(sf::Color::Red);  // Set color for the life bar
        lifeBar.setPosition(50, 70);  // Position the life bar

        window.draw(hungerBar);  // Draw the hunger bar
        window.draw(lifeBar);  // Draw the life bar

        for (const sf::Vector2f& objectPos : otherObjects) {
            sf::CircleShape objectShape(10.f);  // Circle with radius 10
            objectShape.setFillColor(sf::Color::Red);
            objectShape.setPosition(objectPos.x - 5.f, objectPos.y - 5.f);  // Adjust to center the circle
            window.draw(objectShape);  // Draw each object
        }

        window.display();
    }

    return 0;
}