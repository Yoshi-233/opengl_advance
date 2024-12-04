#include "directionalLightShadow.h"
#include "../../../../application/camera/include/orthographicCamera.h"

DirectionalLightShadow::DirectionalLightShadow()
{
        float size = 10.0f;
        this->mCamera = std::make_shared<OrthographicCamera>(-size, size,
                                                             -size, size,
                                                             0.1f, 80.0f);
        this->mRenderTarget = Framebuffer::createShadowFbo(1024, 1024);
}

DirectionalLightShadow::~DirectionalLightShadow() = default;

void DirectionalLightShadow::setRenderTargetSize(int width, int height)
{
        this->mRenderTarget = Framebuffer::createShadowFbo(width, height);
}

glm::mat4 DirectionalLightShadow::getLightMatrix(glm::mat4 lightModelMatrix) const
{
        auto viewMatrix = glm::inverse(lightModelMatrix);
        auto projectionMatrix = this->mCamera->getProjectionMatrix();
        return projectionMatrix * viewMatrix;
}
