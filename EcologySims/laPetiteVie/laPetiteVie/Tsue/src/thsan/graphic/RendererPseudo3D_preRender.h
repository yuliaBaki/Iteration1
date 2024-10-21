#pragma once
#include "RendererPseudo3D.h"

class RendererPseudo3D_preRender : public RendererPseudo3D
{
public:
    RendererPseudo3D_preRender(int width, int height,
        std::string terrainTexturePath,
        std::string depthTerrainTexturePath,
        std::string positionXTerrainTexturePath,
        std::string positionYTerrainTexturePath,
        std::string positionZTerrainTexturePath,
        std::string normalTerrainTexturePath
    );

    virtual sf::Image getImage(EXPORT_IMAGE_TYPE export_image_type) override;

    ~RendererPseudo3D_preRender();



private:
    virtual void renderTerrain() override;
    virtual void setScreenResolution(float screen_width, float screen_height) override;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    bool was_rendered;
   
    std::shared_ptr<sf::Shader> p_shader_fixed_color;
    std::shared_ptr<sf::Shader> p_shader_fixed_depth;
    std::shared_ptr<sf::Shader> p_shader_fixed_position;

    std::shared_ptr<sf::Texture> terrainTexture;
    std::shared_ptr<sf::Texture> depthTerrainTexture;
    std::shared_ptr<sf::Texture> positionXTerrainTexture;
    std::shared_ptr<sf::Texture> positionYTerrainTexture;
    std::shared_ptr<sf::Texture> positionZTerrainTexture;
    std::shared_ptr<sf::Texture> normalTerrainTexture;
};

