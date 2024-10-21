#ifndef SPRITEANIMATION_H
#define SPRITEANIMATION_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <map>
#include <vector>
#include <string>
#include <memory>


class  SpriteAnimation: public sf::Sprite
{
    class AnimationData{
        public:
            AnimationData():curr(0), max(0), totalDuration(sf::Time::Zero), reachedDuration(sf::Time::Zero) {};
            sf::IntRect getCurrentFrameRect(){ return frameRect[curr];};
            sf::Time getCurrentFrameTime(){ return frameTime[curr];};

            void addFrame(sf::IntRect rect, sf::Time time){
                frameRect.emplace_back(rect);
                frameTime.emplace_back(time);
                max++;
                totalDuration += time;
            };
            void reset() {
                curr = 0;
                reachedDuration = sf::Time::Zero;
            }
            void setCurrIndex(unsigned short i){curr = i;}
            unsigned short getCurrIndex(){return curr;}
            unsigned short getSize(){return max;}
            sf::Time getTotalDuration() { return totalDuration; }
            sf::Time getReachedDuration() { return reachedDuration; }
            void nextFrame(){
                if (curr >= max - 1) {
                    if (looping)
                        curr = 0;
                    else
                        reachedDuration = sf::Time::Zero;
                }else
                    curr++;
            };
            void prevFrame(){(curr < 0)   ? curr = max-1: curr--;}
            void loopEnable(bool isEnable) { looping = isEnable; }
            bool isLoop() { return looping; }

        private:
            std::vector<sf::IntRect> frameRect;
            std::vector<sf::Time> frameTime;
            unsigned short curr;
            unsigned short max;
            sf::Time totalDuration;
            sf::Time reachedDuration;
            bool looping{true};
    };

    public:
        void hide();
        void show();
        bool isHidden();

        SpriteAnimation();
        SpriteAnimation(const std::string& SpriteSheetLocation);
        SpriteAnimation(const sf::Texture& texture);

        void init(const std::string& SpriteSheetLocation); //in case of changing spriteSheet
        void init(const sf::Texture& texture); //in case of changing spriteSheet

		void flipX();
		void flipY();

		bool isFlipX();
		bool isFlipY();

        bool hasAnimation(const std::string& name);

        void setCurrAnimation(std::string name);
        std::string getCurrAnimation();

        unsigned int getCurrAnimationFrameIndex();
        void enableLoop(bool enable);
        void resetCurrAnimation();
        void setMainAnimation(std::string name); //no use right now
        std::string getMainAnimation();

        void addFrame(std::string name, sf::IntRect frameRect, sf::Time frameTime);
        sf::IntRect getCurrFrameRect();
        void update(const sf::Time &deltaTime);

		sf::Vector2u getSpriteSheetSize();

        bool hasDepthTexture();
		void setDepthTexture(const std::string& path);
        std::shared_ptr<sf::Texture> GetDepthTexture() const;
        std::string getDepthTexturePath();

        void setKeyColor(sf::Color color);
        sf::Color getKeyColor();

        void setShininess(int shininess);
        int getShininess();

        bool hasEmissionTexture();
        void setEmissionTexture(const std::string& path);
        std::shared_ptr<sf::Texture> getEmissionTexture() const;
        std::string getEmissionTexturePath();

        float getDepthFactor();
        void setDepthFactor(float depthFactor);

        sf::Time getCurrTimeDuration();
        sf::Time getCurrTimeReached();


    private:
        void makeDrawable();
        sf::Color keyColor;
        std::map<std::string, AnimationData> animations;
        std::string currAnimation;
        std::string mainAnimation;
        sf::Sprite m_sprite;
		std::shared_ptr<sf::Texture> depthTexture;
		std::shared_ptr<sf::Texture> emissionTexture;
        std::string emissionTexturePath;
        std::string depthTexturePath;
		sf::Vector2u spriteSheetSize;
		float xFlip;
		float yFlip;
        float depthFactor;
        int shininess;
        bool hidden;
        bool depthEnable;
        bool emissionEnable;

        bool animation_is_done{ true };

		sf::Time tempsAccumuleFrame{sf::Time::Zero};
		sf::Time tempsAccumuleAnimation{sf::Time::Zero};

};

#endif // SPRITEANIMATION_H
