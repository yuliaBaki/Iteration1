#include "ThemeManager.h"
#include "../util/helper.h"
#include <iostream>
#include "TextureManager.h"

namespace RessourceManager {

    ThemeManager::ThemeManager()
    {
        //ctor
    }

    std::shared_ptr<UI::ShaderUIparam> ThemeManager::get(const std::string& path)
    {
        const auto i = p_themes.find(path);
        if (i != p_themes.end())
            return i->second;
        else {
            std::shared_ptr<UI::ShaderUIparam> p_theme = std::make_shared<UI::ShaderUIparam>();
            json j;
            readJson(path, &j);
            paramUI::shaderParam theme_json = nlohmann::json::parse(j.dump());
            p_theme->borderSize[0] = theme_json.border_size[0];
            p_theme->borderSize[1] = theme_json.border_size[1];
            p_theme->borderSize[2] = theme_json.border_size[2];
            p_theme->borderSize[3] = theme_json.border_size[3];

            auto color_bg = theme_json.color_bg;
            p_theme->colorBg[0] = sf::Color(color_bg.at(0).r, color_bg.at(0).g, color_bg.at(0).b, color_bg.at(0).a);
            p_theme->colorBg[1] = sf::Color(color_bg.at(1).r, color_bg.at(1).g, color_bg.at(1).b, color_bg.at(1).a);
            p_theme->colorBg[2] = sf::Color(color_bg.at(2).r, color_bg.at(2).g, color_bg.at(2).b, color_bg.at(2).a);
            p_theme->colorBg[3] = sf::Color(color_bg.at(3).r, color_bg.at(3).g, color_bg.at(3).b, color_bg.at(3).a);

            auto color_border = theme_json.color_border;
            p_theme->colorBorder = sf::Color(color_border.r, color_border.g, color_border.b, color_border.a);

            auto color_border_borders = theme_json.color_border_borders;
            p_theme->colorBorderBorders = sf::Color(color_border_borders.r, color_border_borders.g, color_border_borders.b, color_border_borders.a);

            auto corner_radius = theme_json.corner_radius;
            p_theme->cornerRadius[0] = corner_radius.at(0);
            p_theme->cornerRadius[1] = corner_radius.at(1);
            p_theme->cornerRadius[2] = corner_radius.at(2);
            p_theme->cornerRadius[3] = corner_radius.at(3);

            p_theme->density = theme_json.density;

            auto direction = theme_json.direction;
            p_theme->direction = sf::Vector3f(direction.x, direction.y, direction.z);

            p_theme->enableImage = theme_json.enable_image;
            p_theme->p_texture = RessourceManager::TextureManager::get(theme_json.p_texture);
            p_theme->speed = static_cast<float>(theme_json.speed);

            if (theme_json.type == "fill")
                p_theme->type = UI::Type::fill;
            else
                p_theme->type = UI::Type::none;

            p_themes.insert({ path, p_theme });
            return p_theme;
        }

    }

    void ThemeManager::removeUnused()
    {
        for (auto i = p_themes.begin(); i != p_themes.end();)
            if (i->second.use_count() == 1)
                i = p_themes.erase(i);
            else
                ++i;
    }

    std::unordered_map<std::string, std::shared_ptr<UI::ShaderUIparam>> ThemeManager::p_themes;
}
