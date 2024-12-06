#include "directionalLightCSMShadow.h"
#include "../../../../application/camera/include/orthographicCamera.h"
#include "../../../../application/camera/include/perspectiveCamera.h"
#include "tools.h"

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

glm::mat4 DirectionalLightCSMShadow::getLightMatrix(std::shared_ptr<Camera> &camera, glm::vec3 &LightDir, float near,
                                                    float far)
{
        /* 1.子视锥体的8个角点的世界坐标 */
        auto perpCamera = std::dynamic_pointer_cast<PerspectiveCamera>(camera);
        auto perpViewMatrix = perpCamera->getViewMatrix();
        // 注意这里的near和far是子视锥体的near和far，不是主视锥体的near和far
        auto perpProjMatrix = glm::perspective(glm::radians(perpCamera->mFov),
                                               perpCamera->mAspect, near, far);
        auto corners = Tools::getFrustumCornersWorldSpace(perpProjMatrix * perpViewMatrix);

        /* 2.光源方向, 8个角点的平均值作为光源的位置 */
        glm::vec3 center = glm::vec3(0.0f);
        for(auto corner : corners) {
                center += glm::vec3(corner);
        }
        center /= corners.size();

        auto lightViewMatrix = glm::lookAt(center, center + LightDir,
                                           glm::vec3(0.0f, 1.0f, 0.0f));

        /* 3.光源方向的包围盒， need 8个角点在光源方向的投影 */
        float minX = std::numeric_limits<float>::max();
        float maxX = std::numeric_limits<float>::lowest();
        float minY = std::numeric_limits<float>::max();
        float maxY = std::numeric_limits<float>::lowest();
        float minZ = std::numeric_limits<float>::max();
        float maxZ = std::numeric_limits<float>::lowest();
        for(const auto& corner : corners) {
                // 三维坐标转光源视角投影坐标
                auto projCorner = lightViewMatrix * corner;
                minX = std::min(minX, projCorner.x);
                maxX = std::max(maxX, projCorner.x);
                minY = std::min(minY, projCorner.y);
                maxY = std::max(maxY, projCorner.y);
                minZ = std::min(minZ, projCorner.z);
                maxZ = std::max(maxZ, projCorner.z);
        }
        // 注意此时的子视锥体的光源方向的透视盒子太短（相对相机），离开相机太远，有遮挡物就遮挡不到了

        /* 4.调整（包围盒以外的物体有可能遮挡到光源，所以要扩大包围盒） */
        minZ *= 10.0f;
        maxZ *= 10.0f;

        /* 5. 计算光源视角投影矩阵 */
        // 这里是以光源相机为中心的正交投影矩阵, near和far都是距离值
        // 但经过光源相机viewMatrix后的camera, minZ是负数，对应far的方向（相机看向的方向）， maxZ是正数，对应相机后面的方向
        auto lightProjMatrix = glm::ortho(minX, maxX,
                                          minY, maxY,
                                          -maxZ, -minZ);

        return lightProjMatrix * lightViewMatrix;
}

std::vector<glm::mat4> DirectionalLightCSMShadow::getLightMatrices(std::shared_ptr<Camera> &camera, glm::vec3 &LightDir,
                                                                   std::vector<float> &clips)
{
        std::vector<glm::mat4> lightMatrices;
        for(int i = 0; i < clips.size() - 1 ; i++) {
                float near = clips[i];
                float far = clips[i+1];
                glm::mat4 lightMatrix = getLightMatrix(camera, LightDir, near, far);
                lightMatrices.push_back(lightMatrix);
        }
        return lightMatrices;
}
