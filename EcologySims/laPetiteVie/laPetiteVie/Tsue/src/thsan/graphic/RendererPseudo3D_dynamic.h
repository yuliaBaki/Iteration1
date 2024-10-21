#pragma once
#include "RendererPseudo3D.h"

class RendererPseudo3D_dynamic : public RendererPseudo3D
{
  
    public:
        RendererPseudo3D_dynamic(int width, int height);

        virtual sf::Image getImage(EXPORT_IMAGE_TYPE export_image_type) override;

        ~RendererPseudo3D_dynamic() = default;

    private:
        virtual void renderTerrain() override;
        virtual void setScreenResolution(float screen_width, float screen_height) override;
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

