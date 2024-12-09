#include "pointLightShadow.h"
#include "../../../../application/camera/include/orthographicCamera.h"

PointLightShadow::PointLightShadow()
{
        float size = 50.0f;
        this->mCamera = std::make_shared<OrthographicCamera>(-size, size,
                                                             -size, size,
                                                             0.1f, 200.0f);
        this->mRenderTarget = Framebuffer::createShadowFbo(1024, 1024);
}

PointLightShadow::~PointLightShadow() = default;

void PointLightShadow::setRenderTargetSize(int width, int height)
{
        this->mRenderTarget = Framebuffer::createShadowFbo(width, height);
}

glm::mat4 PointLightShadow::getLightMatrix(glm::mat4 lightModelMatrix) const
{
        auto viewMatrix = glm::inverse(lightModelMatrix);
        auto projectionMatrix = this->mCamera->getProjectionMatrix();
        return projectionMatrix * viewMatrix;
}
