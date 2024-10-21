#ifndef CAMERARENDERCOMPONENT_H
#define CAMERARENDERCOMPONENT_H

#include <thsan/gameObject/component/RenderComponent.h>

class CameraRenderComponent : public RenderComponent
{
    public:
        CameraRenderComponent();

        void init(Scene& scene) override;
        void update(Scene& scene, const sf::Time& dt, sf::RenderTarget &target, RendererPseudo3D& rendererPseudo3D) override;

        ~CameraRenderComponent();
    private:
        CameraData* cam_data;

};

#endif // CAMERARENDERCOMPONENT_H
