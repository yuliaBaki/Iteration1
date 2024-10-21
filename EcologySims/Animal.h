#pragma once
class Animal                      
{
public:                      
    Animal(std::shared_ptr<std::vector<sf::Texture>> gifImages,
           sf::Vector2f currentPos,
           sf::Vector2f targetPos,
           sf::Clock frameClock,
           sf::Clock hungerClock);
    Animal(const Cat& copy_from);
    ~Animal();                   

    const &std::vector<sf::Texture> getGigImages();       
    void SetAge(int age);     
    void Meow();
private:                      
 //   int itsAge;                // member variable
   // char* string;
    std::shared_ptr<std::vector<sf::Texture>> gifFrames;
    sf::Vector2f currentPos;
    sf::Vector2f targetPos;
    sf::Clock frameClock;  // To control the frame switching time
    sf::Clock hungerClock; // To reduce hunger over time
    sf::Sprite objectSprite;
    bool isFlipped = false;  // To check if the object is currently moving
    // Life and Hunger system
    float life = 100.0f;  // Maximum life is 100
    float hunger = 100.0f;  // Hunger starts at 100 and reduces over time
    sf::RectangleShape hungerBar(sf::Vector2f(hunger, 10));  // Create a bar for hunger
    sf::RectangleShape lifeBar(sf::Vector2f(life, 10));
};
