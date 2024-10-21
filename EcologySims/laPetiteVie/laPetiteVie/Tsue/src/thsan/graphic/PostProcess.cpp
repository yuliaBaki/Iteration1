#include "PostProcess.h"

#include "../ressourceManager/ShaderManager.h"
#include "../util/helperMath.h"
#include <iostream>
PostProcess::PostProcess(sf::Vector2u resolution):
    resolution(resolution),
    snowfall_param{ sf::Glsl::Vec2(-0.1f, -1.f), sf::Glsl::Vec2(1.f,1.f), 0.1f, 2.f, 2.f, 2.f },
    letterbox_param{
        Letterbox_param::Type::step,
        {
            Letterbox_param::Bar::top,
            Letterbox_param::Bar::bottom,
            Letterbox_param::Bar::right,
            Letterbox_param::Bar::left
        },
        Blend::none,
        sf::Color::Black, sf::Color::Black, sf::Color::Black, sf::Color::Black,
        0.1f, 0.1f, 0.05f, 0.05f, 0.f, 0.f, 0.f, 0.f
    } ,
    ambient(sf::Color::Cyan)
{
    texture_output.create(resolution.x, resolution.y);

    shader_pp = RessourceManager::ShaderManager::get("media/shader/postProcess/postprocess.vert", "media/shader/postProcess/postprocess.frag");
    effects.fill(-1);
    opacity.fill(0.f);
    using lBar = Letterbox_param::Bar;
    letterbox_param.priority_letterbox = { lBar::top, lBar::bottom, lBar::right, lBar::left };
    letterbox_param.blend = Blend::none;
}

void PostProcess::update(const sf::Time& deltaTime)
{
    elapsedTime += deltaTime;
    this->deltaTime = deltaTime;

    letterbox_priority_checker();

    sf::Vector2f tex_size = sf::Vector2f(texture_color.getSize());

    if (tex_size.x != 0.f && tex_size.y != 0.f) {
        float ratio_x = resolution.x / tex_size.x;
        float ratio_y = resolution.y / tex_size.y;

        sprite_output.setScale(ratio_x, ratio_y);
    }
}

float PostProcess::getOpacity(Effect effect)
{
    return opacity[static_cast<int>(effect)];
}

void PostProcess::setOpacity(Effect effect, float opacity)
{
    this->opacity[static_cast<int>(effect)] = opacity;
}

void PostProcess::setInputColor(const sf::Texture texture)
{
    texture_color = texture;
    sprite_output.setTexture(texture_color);

    sf::Vector2u size = texture_color.getSize();
    //sprite_output.setTextureRect(sf::IntRect(0, size.y, size.x, -1 * static_cast<int>(size.y)));
}

void PostProcess::setInputDepth(const sf::Texture texture)
{
    texture_depth = texture;
}

void PostProcess::setResolution(sf::Vector2u resolution)
{
    this->resolution = resolution;
}

sf::Vector2u PostProcess::getResolution()
{
    return resolution;
}

void PostProcess::setDrectionalLightAmbient(sf::Color color)
{
    ambient = color;
}

void PostProcess::bind_effect_to_layer(Effect effect, int position)
{
    effects[position] = static_cast<int>(effect);
}

void PostProcess::enable_effect(Effect effect, float opacity)
{
    int i = static_cast<int>(effect);
    this->opacity[i] = opacity;
    effects_enable[i] = true;
}

void PostProcess::disable_effect(Effect effect)
{
    effects_enable[static_cast<int>(effect)] = false;
}

void PostProcess::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.shader = &*shader_pp;
    states.texture = &texture_color;
    states.transform = getTransform();

    for (int i = 0; i < MAX_EFFECT; i++) {
        int curr_effect = effects[i];

        if (effects_enable[curr_effect]) {
            shader_pp->setUniform("opacity[" + std::to_string(curr_effect) + "]", opacity[curr_effect]);
            shader_pp->setUniform("effects[" + std::to_string(i) + "]", effects[i]);
        }
        else
            shader_pp->setUniform("effects[" + std::to_string(i) + "]", -1);
    }

    shader_pp->setUniform("width", static_cast<float>(texture_color.getSize().x));
    shader_pp->setUniform("height", static_cast<float>(texture_color.getSize().y));

    shader_pp->setUniform("texture_depth", texture_depth);

    //snowfall parameter
    shader_pp->setUniform("directionalLight_ambient", sf::Glsl::Vec4(ambient));
    shader_pp->setUniform("wind_direction",snowfall_param.wind_direction);
    shader_pp->setUniform("speed", snowfall_param.speed);
    shader_pp->setUniform("size_rate", snowfall_param.size_rate);
    shader_pp->setUniform("fg_layers",snowfall_param.fg_layers);
    shader_pp->setUniform("bg_layers", snowfall_param.bg_layers);
    shader_pp->setUniform("layer_gap", snowfall_param.layer_gap);

    //letterbox param
    shader_pp->setUniform("time", elapsedTime.asSeconds());

    shader_pp->setUniform("type", static_cast<int>(letterbox_param.type));

    for (int i = 0; i < 4; i++)
        shader_pp->setUniform("priority_letterbox[" + std::to_string(i) + "]", static_cast<int>(letterbox_param.priority_letterbox[i]));

    shader_pp->setUniform("letterbox_blend", static_cast<int>(letterbox_param.blend));

    shader_pp->setUniform("top_bar_color", sf::Glsl::Vec4(letterbox_param.top_bar_color));
    shader_pp->setUniform("bottom_bar_color", sf::Glsl::Vec4(letterbox_param.bottom_bar_color));
    shader_pp->setUniform("left_bar_color", sf::Glsl::Vec4(letterbox_param.left_bar_color));
    shader_pp->setUniform("right_bar_color", sf::Glsl::Vec4(letterbox_param.right_bar_color));

    shader_pp->setUniform("bar_screen_ratio_top",letterbox_param.bar_screen_ratio_top);
    shader_pp->setUniform("bar_screen_ratio_bottom", letterbox_param.bar_screen_ratio_bottom);
    shader_pp->setUniform("bar_screen_ratio_left", letterbox_param.bar_screen_ratio_left);
    shader_pp->setUniform("bar_screen_ratio_right", letterbox_param.bar_screen_ratio_right);

    shader_pp->setUniform("fading_range_top", letterbox_param.fading_range_top);
    shader_pp->setUniform("fading_range_bottom", letterbox_param.fading_range_bottom);
    shader_pp->setUniform("fading_range_left", letterbox_param.fading_range_left);
    shader_pp->setUniform("fading_range_right", letterbox_param.fading_range_right);

    target.draw(sprite_output, states);
}

void PostProcess::letterbox_priority_checker()
{
    std::vector<Letterbox_param::Bar> temp;
    for (Letterbox_param::Bar bar : letterbox_param.priority_letterbox) {
        auto p = std::find(temp.begin(), temp.end(), bar);
        if (p == temp.end())
            temp.push_back(bar);
        else {
            using lBar = Letterbox_param::Bar;
            letterbox_param.priority_letterbox = { lBar::top, lBar::right, lBar::bottom, lBar::left };
            break;
        }
    }
}

bool PostProcess::isEnable() 
{
    return enabled;
}

void PostProcess::enable()
{
    enabled = true;
}

void PostProcess::disable()
{
    enabled = false;
}

void PostProcess::clear()
{
    snowfall_param = { sf::Glsl::Vec2(-0.1f, -1.f), sf::Glsl::Vec2(1.f,1.f), 0.1f, 2.f, 2.f, 2.f };
    letterbox_param = {
        Letterbox_param::Type::step,
        {
            Letterbox_param::Bar::top,
            Letterbox_param::Bar::bottom,
            Letterbox_param::Bar::right,
            Letterbox_param::Bar::left
        },
        Blend::none,
        sf::Color::Black, sf::Color::Black, sf::Color::Black, sf::Color::Black,
        0.f, 0.f, 0.0f, 0.0f, 0.f, 0.f, 0.f, 0.f
    };

    disable_effect(Effect::snowfall);
    disable_effect(Effect::letterbox);
}
