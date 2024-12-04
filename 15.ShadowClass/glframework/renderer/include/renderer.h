//
// Created by Shaozheming on 2024/9/19.
//

#ifndef RENDERER_RENDERER_H
#define RENDERER_RENDERER_H

#include "../../include/core.h"
#include "../../include/scene.h"
#include <vector>
#include "../../mesh/include/mesh.h"
#include "../../include/shader.hpp"
#include "../../light/include/directionalLight.h"
#include "../../light/include/pointLight.h"
#include "../../light/include/spotLight.h"
#include "../../light/include/ambientLight.h"
#include "../../materials/include/material.h"
#include "../../materials/include/phongMaterial.h"
#include "../../materials/include/phongEnvMaterial.h"
#include "../../materials/include/phongInstancedMaterial.h"
#include "../../materials/include/grassInstancedMaterial.h"

#include "../../../application/camera/include/camera.h"

#include "../../framebuffer/include/framebuffer.h"


class Renderer {
public:
        Renderer();

        ~Renderer();

        // 聚光灯 + 点光源 + 平行光
        void render(const std::shared_ptr<Scene> &scene,
                    const std::shared_ptr<Camera> &camera,
                    const std::shared_ptr<DirectionalLight> &directionalLight,
                    const std::vector<std::shared_ptr<PointLight>> &pointLights,
                    const std::shared_ptr<SpotLight> &spotLight,
                    const std::shared_ptr<AmbientLight> &ambientLight,
                    unsigned int fbo = 0);

        // 点光源
        void render(const std::shared_ptr<Scene> &scene,
                    const std::shared_ptr<Camera> &camera,
                    const std::shared_ptr<PointLight> &pointLight,
                    const std::shared_ptr<AmbientLight> &ambientLight,
                    unsigned int fbo = 0);

        // 平行光
        void render(const std::shared_ptr<Scene> &scene,
                    const std::shared_ptr<Camera> &camera,
                    const std::shared_ptr<DirectionalLight> &directionalLight,
                    const std::shared_ptr<AmbientLight> &ambientLight,
                    unsigned int fbo = 0);


        void renderObject(const std::shared_ptr<Object> &object,
                          const std::shared_ptr<Camera> &camera,
                          const std::shared_ptr<DirectionalLight> &directionalLight,
                          const std::vector<std::shared_ptr<PointLight>> &pointLights,
                          const std::shared_ptr<SpotLight> &spotLight,
                          const std::shared_ptr<AmbientLight> &ambientLight);


        void renderShadowMap(const std::vector<std::shared_ptr<Mesh>> &meshes,
                             const std::shared_ptr<DirectionalLight> &directionalLight,
                             const std::shared_ptr<Framebuffer> &fbo);
public:
        static void setClearColor(const glm::vec3 &color);

public:
        /* 全局材质 */
        std::shared_ptr<Material> mGlobalMaterial{nullptr};

private:
        /* 各种材质渲染func */
        static void phongMaterialRender(const std::shared_ptr<Shader> &shaderPtr,
                                        const std::shared_ptr<Material> &material,
                                        const std::shared_ptr<Camera> &camera,
                                        const std::shared_ptr<Mesh> &mesh,
                                        const std::shared_ptr<DirectionalLight> &directionalLight,
                                        const std::vector<std::shared_ptr<PointLight>> &pointLights,
                                        const std::shared_ptr<SpotLight> &spotLight,
                                        const std::shared_ptr<AmbientLight> &ambientLight);

        static void phongEnvMaterialRender(const std::shared_ptr<Shader> &shaderPtr,
                                           const std::shared_ptr<Material> &material,
                                           const std::shared_ptr<Camera> &camera,
                                           const std::shared_ptr<Mesh> &mesh,
                                           const std::shared_ptr<DirectionalLight> &directionalLight,
                                           const std::vector<std::shared_ptr<PointLight>> &pointLights,
                                           const std::shared_ptr<SpotLight> &spotLight,
                                           const std::shared_ptr<AmbientLight> &ambientLight);

        static void phongInstancedMaterialRender(const std::shared_ptr<Shader> &shaderPtr,
                                                 const std::shared_ptr<Material> &material,
                                                 const std::shared_ptr<Camera> &camera,
                                                 const std::shared_ptr<Mesh> &mesh,
                                                 const std::shared_ptr<DirectionalLight> &directionalLight,
                                                 const std::vector<std::shared_ptr<PointLight>> &pointLights,
                                                 const std::shared_ptr<SpotLight> &spotLight,
                                                 const std::shared_ptr<AmbientLight> &ambientLight);

        static void grassInstancedMaterialRender(const std::shared_ptr<Shader> &shaderPtr,
                                                 const std::shared_ptr<Material> &material,
                                                 const std::shared_ptr<Camera> &camera,
                                                 const std::shared_ptr<Mesh> &mesh,
                                                 const std::shared_ptr<DirectionalLight> &directionalLight,
                                                 const std::vector<std::shared_ptr<PointLight>> &pointLights,
                                                 const std::shared_ptr<SpotLight> &spotLight,
                                                 const std::shared_ptr<AmbientLight> &ambientLight);

        static void opacityMaskMaterialRender(const std::shared_ptr<Shader> &shaderPtr,
                                              const std::shared_ptr<Material> &material,
                                              const std::shared_ptr<Camera> &camera,
                                              const std::shared_ptr<Mesh> &mesh,
                                              const std::shared_ptr<DirectionalLight> &directionalLight,
                                              const std::vector<std::shared_ptr<PointLight>> &pointLights,
                                              const std::shared_ptr<SpotLight> &spotLight,
                                              const std::shared_ptr<AmbientLight> &ambientLight);

private:
        /* advanced render */
        static void phongNormalMaterialRender(const std::shared_ptr<Shader> &shaderPtr,
                                              const std::shared_ptr<Material> &material,
                                              const std::shared_ptr<Camera> &camera,
                                              const std::shared_ptr<Mesh> &mesh,
                                              const std::shared_ptr<DirectionalLight> &directionalLight,
                                              const std::vector<std::shared_ptr<PointLight>> &pointLights,
                                              const std::shared_ptr<SpotLight> &spotLight,
                                              const std::shared_ptr<AmbientLight> &ambientLight);

        static void phongParallaxMaterialRender(const std::shared_ptr<Shader> &shaderPtr,
                                                const std::shared_ptr<Material> &material,
                                                const std::shared_ptr<Camera> &camera,
                                                const std::shared_ptr<Mesh> &mesh,
                                                const std::shared_ptr<DirectionalLight> &directionalLight,
                                                const std::vector<std::shared_ptr<PointLight>> &pointLights,
                                                const std::shared_ptr<SpotLight> &spotLight,
                                                const std::shared_ptr<AmbientLight> &ambientLight);

        void phongShadowMaterialRender(const std::shared_ptr<Shader> &shaderPtr,
                                                const std::shared_ptr<Material> &material,
                                                const std::shared_ptr<Camera> &camera,
                                                const std::shared_ptr<Mesh> &mesh,
                                                const std::shared_ptr<DirectionalLight> &directionalLight,
                                                const std::vector<std::shared_ptr<PointLight>> &pointLights,
                                                const std::shared_ptr<SpotLight> &spotLight,
                                                const std::shared_ptr<AmbientLight> &ambientLight);

private:
        // 渲染内部调用， 根据类型不同挑选
        std::shared_ptr<Shader> pickShader(MaterialType type);

        // 遍历所有mesh，区分透明不透明
        void projectObject(const std::shared_ptr<Object> &object);

private:
        /* 各种光照 */
        static void processDirectionLight(const std::shared_ptr<Shader> &shaderPtr,
                                          const std::shared_ptr<DirectionalLight> &directionalLight);

        static void processPointLight(const std::shared_ptr<Shader> &shaderPtr,
                                      const std::shared_ptr<PointLight> &pointLight);

        static void processPointLight(const std::shared_ptr<Shader> &shaderPtr,
                                      const std::vector<std::shared_ptr<PointLight>> &pointLights);

        static void processSpotLight(const std::shared_ptr<Shader> &shaderPtr,
                                     const std::shared_ptr<SpotLight> &spotLight);

        static void processSpotLight(const std::shared_ptr<Shader> &shaderPtr,
                                     const std::vector<std::shared_ptr<SpotLight>> &spotLights);

        template<typename T>
        static void processSpecularMask(const std::shared_ptr<Shader> &shaderPtr,
                                        T *phongMaterial);


private:
        static void processCommonInfo(const std::shared_ptr<Shader> &shaderPtr,
                                      const std::shared_ptr<Camera> &camera,
                                      const std::shared_ptr<Mesh> &mesh);

        static void processAmbientShiness(const std::shared_ptr<Shader> &shaderPtr,
                                          const std::shared_ptr<AmbientLight> &ambientLight);

private:
        /* 设置状态 */
        static void setDepthStatus(const std::shared_ptr<Material> &material);

        static void setPolygonOffsetStatus(const std::shared_ptr<Material> &material);

        static void setStencilStatus(const std::shared_ptr<Material> &material);

        static void setBlendStatus(const std::shared_ptr<Material> &material);

        void setGLInitStatus(const std::shared_ptr<Scene> &scene, const std::shared_ptr<Camera> &camera);

        static void setGLStatus(const std::shared_ptr<Material> &material);

        static void setFaceCullingStatus(const std::shared_ptr<Material> &material);

private:
        /* 处理不同材质对应相机的渲染 */
        static void processWhiteMaterial(const std::shared_ptr<Shader> &shaderPtr,
                                         const std::shared_ptr<Mesh> &mesh,
                                         const std::shared_ptr<Camera> &camera);

        static void processDepthMaterial(const std::shared_ptr<Shader> &shaderPtr,
                                         const std::shared_ptr<Mesh> &mesh,
                                         const std::shared_ptr<Camera> &camera);

        static void processCubeMaterial(const std::shared_ptr<Shader> &shaderPtr,
                                        const std::shared_ptr<Mesh> &mesh,
                                        const std::shared_ptr<Camera> &camera);


private:
        // 生成多种shader, 根据材质不同选择
        std::shared_ptr<Shader> mPhoneShader{nullptr};
        std::shared_ptr<Shader> mPhoneEnvShader{nullptr};
        std::shared_ptr<Shader> mPhoneInstancedShader{nullptr};
        std::shared_ptr<Shader> mGrassInstancedShader{nullptr};
        std::shared_ptr<Shader> mWhiteShader{nullptr};
        std::shared_ptr<Shader> mDepthShader{nullptr};
        std::shared_ptr<Shader> mOpacityMaskShader{nullptr};
        std::shared_ptr<Shader> mScreenShader{nullptr};
        std::shared_ptr<Shader> mCubeMapShader{nullptr};

        /* advance */
        std::shared_ptr<Shader> mPhongNormalShader{nullptr};
        std::shared_ptr<Shader> mPhongParallaxShader{nullptr};
        std::shared_ptr<Shader> mShadowShader{nullptr};
        std::shared_ptr<Shader> mPhongShadowShader{nullptr};

private:
        /* 不透明物体和透明物体的队列 */
        // 注意：每一次渲染，都会清空队列
        std::vector<std::shared_ptr<Mesh>> mOpacityObjects{};
        std::vector<std::shared_ptr<Mesh>> mTransparentObjects{};


};

template<typename T>
void Renderer::processSpecularMask(const std::shared_ptr<Shader> &shaderPtr,
                                   T *phongMaterial)
{
        if (phongMaterial->getSpecularMask() != nullptr) {
                // 高光蒙板
                shaderPtr->setFloat("specularMaskFlag", 1.0f);
                shaderPtr->setInt("specularMaskSampler", 1);
                phongMaterial->getSpecularMask()->bind();
        } else {
                shaderPtr->setFloat("specularMaskFlag", 0.0f);
        }
}

#endif //RENDERER_RENDERER_H
