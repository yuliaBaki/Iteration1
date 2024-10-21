#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
#include <memory>
#include <array>



class PostProcess: public sf::Drawable, public sf::Transformable {
public:
    enum class Blend {
        none = 0,
        additive = 1,
        subtract = 2,
        multiply = 3,
        divide = 4,
        darken_only = 5,
        lighten_only = 6
    };

    struct Snowfall_param{
        sf::Glsl::Vec2 wind_direction;
        sf::Glsl::Vec2 speed;

        float size_rate;
        float fg_layers;
        float bg_layers;
        float layer_gap;
    };

    struct Letterbox_param {
        enum class Type {
            step = 0,
            smoothstep = 1
        };

        enum class Bar {
            top = 0,
            right = 1,
            bottom = 2,
            left = 3
        };

        Type type;
        std::array<Bar, 4> priority_letterbox; // do not put the same value twice, will force clockwise otherwise.
        Blend blend;
        sf::Color top_bar_color;
        sf::Color bottom_bar_color;
        sf::Color left_bar_color;
        sf::Color right_bar_color;

        float bar_screen_ratio_top;
        float bar_screen_ratio_bottom;
        float bar_screen_ratio_left;
        float bar_screen_ratio_right;

        float fading_range_top;
        float fading_range_bottom;
        float fading_range_left;
        float fading_range_right;
    };

    enum class Effect
    {
        snowfall = 0,
        letterbox = 1,
        LAST
    };

    Snowfall_param snowfall_param;
    Letterbox_param letterbox_param;

	PostProcess(sf::Vector2u resolution);
	~PostProcess() = default;

	void update(const sf::Time& deltaTime);

    float getOpacity(Effect effect);
    void setOpacity(Effect effect, float opacity);

    void setInputColor(const sf::Texture texture);
    void setInputDepth(const sf::Texture texture);

    void setResolution(sf::Vector2u resolution);
    sf::Vector2u  getResolution();

    void setDrectionalLightAmbient(sf::Color color);

    void bind_effect_to_layer(Effect effect, int position);

    // interpolation values set at default 0 second will cancel out the interpolation
    void enable_effect(Effect effect, float opacity = 1.0f);
    void disable_effect(Effect effect);

    bool isEnable();

    void enable();
    void disable();

    void clear();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void letterbox_priority_checker();

    static const int MAX_EFFECT = 10;
    std::array<int, MAX_EFFECT> effects;
    std::array<float, MAX_EFFECT> opacity;
    bool effects_enable[MAX_EFFECT]{ false };
    bool enabled{ false };

    sf::Vector2u resolution;

    sf::Color ambient;

    sf::Sprite sprite_output;
    sf::Texture texture_output;
    sf::Texture texture_depth;
    sf::Texture texture_color;

    sf::Time elapsedTime{sf::Time::Zero};
    sf::Time deltaTime{sf::Time::Zero};

	std::shared_ptr<sf::Shader> shader_pp;
};