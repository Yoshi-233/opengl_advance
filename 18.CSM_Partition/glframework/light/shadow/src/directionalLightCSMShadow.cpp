#include "directionalLightCSMShadow.h"
#include "../../../../application/camera/include/orthographicCamera.h"

DirectionalLightCSMShadow::DirectionalLightCSMShadow()
{
        float size = 50.0f;
        this->mCamera = std::make_shared<OrthographicCamera>(-size, size,
                                                             -size, size,
                                                             0.1f, 200.0f);
        this->mRenderTarget = Framebuffer::createShadowFbo(1024, 1024);
}

DirectionalLightCSMShadow::~DirectionalLightCSMShadow() = default;

void DirectionalLightCSMShadow::setRenderTargetSize(int width, int height)
{
        this->mRenderTarget = Framebuffer::createShadowFbo(width, height);
}

void DirectionalLightCSMShadow::generateCascadeLayers(std::vector<float> &layers, float near, float far) const
{
        layers.clear();

        for(int i = 0; i <= this->mLayerCount; i++) {
                float layer = near * glm::pow(far / near, (float)i / (float)this->mLayerCount);
                layers.push_back(layer);
        }
}
