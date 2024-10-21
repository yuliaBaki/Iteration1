#pragma once

#include <unordered_map>
#include <memory>
#include <SFML/Graphics.hpp>
#include "../UI/Panel.h"
#include "../vendors/nlohmann/json.hpp"


namespace paramUI {
    using nlohmann::json;

    inline json get_untyped(const json& j, const char* property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json& j, std::string property) {
        return get_untyped(j, property.data());
    }

    struct color_b {
        int64_t r;
        int64_t g;
        int64_t b;
        int64_t a;
    };

    struct direction {
        double x;
        double y;
        double z;
    };

    struct shaderParam {
        std::vector<double> border_size;
        std::vector<color_b> color_bg;
        color_b color_border;
        color_b color_border_borders;
        std::vector<double> corner_radius;
        std::string type;
        bool enable_image;
        direction direction;
        int64_t iteration;
        int64_t density;
        std::string p_texture;
        double speed;
    };
}

namespace nlohmann {
    void from_json(const json& j, paramUI::color_b& x);
    void to_json(json& j, const paramUI::color_b& x);

    void from_json(const json& j, paramUI::direction& x);
    void to_json(json& j, const paramUI::direction& x);

    void from_json(const json& j, paramUI::shaderParam& x);
    void to_json(json& j, const paramUI::shaderParam& x);

    inline void from_json(const json& j, paramUI::color_b& x) {
        x.r = j.at("r").get<int64_t>();
        x.g = j.at("g").get<int64_t>();
        x.b = j.at("b").get<int64_t>();
        x.a = j.at("a").get<int64_t>();
    }

    inline void to_json(json& j, const paramUI::color_b& x) {
        j = json::object();
        j["r"] = x.r;
        j["g"] = x.g;
        j["b"] = x.b;
        j["a"] = x.a;
    }

    inline void from_json(const json& j, paramUI::direction& x) {
        x.x = j.at("x").get<double>();
        x.y = j.at("y").get<double>();
        x.z = j.at("z").get<double>();
    }

    inline void to_json(json& j, const paramUI::direction& x) {
        j = json::object();
        j["x"] = x.x;
        j["y"] = x.y;
        j["z"] = x.z;
    }

    inline void from_json(const json& j, paramUI::shaderParam& x) {
        x.border_size = j.at("borderSize").get<std::vector<double>>();
        x.color_bg = j.at("colorBg").get<std::vector<paramUI::color_b>>();
        x.color_border = j.at("colorBorder").get<paramUI::color_b>();
        x.color_border_borders = j.at("colorBorderBorders").get<paramUI::color_b>();
        x.corner_radius = j.at("cornerRadius").get<std::vector<double>>();
        x.type = j.at("type").get<std::string>();
        x.enable_image = j.at("enableImage").get<bool>();
        x.direction = j.at("direction").get<paramUI::direction>();
        x.iteration = j.at("iteration").get<int64_t>();
        x.density = j.at("density").get<int64_t>();
        x.p_texture = j.at("p_texture").get<std::string>();
        x.speed = j.at("speed").get<double>();
    }

    inline void to_json(json& j, const paramUI::shaderParam& x) {
        j = json::object();
        j["borderSize"] = x.border_size;
        j["colorBg"] = x.color_bg;
        j["colorBorder"] = x.color_border;
        j["colorBorderBorders"] = x.color_border_borders;
        j["cornerRadius"] = x.corner_radius;
        j["type"] = x.type;
        j["enableImage"] = x.enable_image;
        j["direction"] = x.direction;
        j["iteration"] = x.iteration;
        j["density"] = x.density;
        j["p_texture"] = x.p_texture;
        j["speed"] = x.speed;
    }
    

}

namespace RessourceManager {
    class ThemeManager
    {
    public:
        ThemeManager();
        static std::shared_ptr<UI::ShaderUIparam> get(const std::string& path);
        static void removeUnused();
    private:
        static std::unordered_map<std::string, std::shared_ptr<UI::ShaderUIparam>> p_themes;
    };
}
