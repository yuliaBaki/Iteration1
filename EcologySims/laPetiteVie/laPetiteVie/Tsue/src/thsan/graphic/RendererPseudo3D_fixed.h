#pragma once
#include "RendererPseudo3D.h"

class RendererPseudo3D_fixed: public RendererPseudo3D
{
public:
    RendererPseudo3D_fixed(int width, int height);

    virtual sf::Image getImage(EXPORT_IMAGE_TYPE export_image_type) override;

    ~RendererPseudo3D_fixed();


private:
    virtual void renderTerrain() override;
    virtual void setScreenResolution(float screen_width, float screen_height) override;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    bool was_rendered;

    std::shared_ptr<sf::Shader> p_shader_fixed_color;
    std::shared_ptr<sf::Shader> p_shader_fixed_depth;
    std::shared_ptr<sf::Shader> p_shader_fixed_position;

    sf::RenderTexture terrainTexture;
    sf::RenderTexture depthTerrainTexture;
    sf::RenderTexture positionXTerrainTexture;
    sf::RenderTexture positionYTerrainTexture;
    sf::RenderTexture positionZTerrainTexture;
    sf::RenderTexture emissionBlurrhTexture;
    sf::RenderTexture emissionBlurrPass1hTexture;
    sf::RenderTexture emissionBlurrPass2hTexture;
};

