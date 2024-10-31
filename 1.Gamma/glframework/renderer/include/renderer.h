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

public:
        static void setClearColor(const glm::vec3 &color);

public:
        /* 全局材质 */
        std::shared_ptr<Material> mGlobalMaterial{nullptr};

private:
        // 渲染内部调用， 根据类型不同挑选
        std::shared_ptr<Shader> pickShader(MaterialType type);

        // 遍历所有mesh，区分透明不透明
        void projectObject(const std::shared_ptr<Object> &object);

private:
        /* 各种光照 */
        static inline void processDirectionLight(const std::shared_ptr<Shader> &shaderPtr,
                                                 const std::shared_ptr<DirectionalLight> &directionalLight);

        static inline void processPointLight(const std::shared_ptr<Shader> &shaderPtr,
                                             const std::shared_ptr<PointLight> &pointLight);

        static inline void processPointLight(const std::shared_ptr<Shader> &shaderPtr,
                                             const std::vector<std::shared_ptr<PointLight>> &pointLights);

        static inline void processSpotLight(const std::shared_ptr<Shader> &shaderPtr,
                                            const std::shared_ptr<SpotLight> &spotLight);

        static inline void processSpotLight(const std::shared_ptr<Shader> &shaderPtr,
                                            const std::vector<std::shared_ptr<SpotLight>> &spotLights);

        static inline void processSpecularMask(const std::shared_ptr<Shader> &shaderPtr,
                                               PhongMaterial *phongMaterial);

        static inline void processSpecularMask(const std::shared_ptr<Shader> &shaderPtr,
                                               PhongEnvMaterial *phongMaterial);

        static inline void processSpecularMask(const std::shared_ptr<Shader> &shaderPtr,
                                               PhongInstancedMaterial *phongMaterial);

        static inline void processSpecularMask(const std::shared_ptr<Shader> &shaderPtr,
                                               GrassInstancedMaterial *phongMaterial);

private:
        static inline void processCommonInfo(const std::shared_ptr<Shader> &shaderPtr,
                                             const std::shared_ptr<Camera> &camera,
                                             const std::shared_ptr<Mesh> &mesh);

        static inline void processAmbientShiness(const std::shared_ptr<Shader> &shaderPtr,
                                                 const std::shared_ptr<AmbientLight> &ambientLight);

private:
        static inline void setDepthStatus(const std::shared_ptr<Material> &material);

        static inline void setPolygonOffsetStatus(const std::shared_ptr<Material> &material);

        static inline void setStencilStatus(const std::shared_ptr<Material> &material);

        static inline void setBlendStatus(const std::shared_ptr<Material> &material);

        void setGLInitStatus(const std::shared_ptr<Scene> &scene, const std::shared_ptr<Camera> &camera);

        static inline void setGLStatus(const std::shared_ptr<Material> &material);

        static inline void setFaceCullingStatus(const std::shared_ptr<Material> &material);

private:
        static inline void processWhiteMaterial(const std::shared_ptr<Shader> &shaderPtr,
                                                const std::shared_ptr<Mesh> &mesh,
                                                const std::shared_ptr<Camera> &camera);

        static inline void processDepthMaterial(const std::shared_ptr<Shader> &shaderPtr,
                                                const std::shared_ptr<Mesh> &mesh,
                                                const std::shared_ptr<Camera> &camera);

        static inline void processCubeMaterial(const std::shared_ptr<Shader> &shaderPtr,
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

private:
        /* 不透明物体和透明物体的队列 */
        // 注意：每一次渲染，都会清空队列
        std::vector<std::shared_ptr<Mesh>> mOpacityObjects{};
        std::vector<std::shared_ptr<Mesh>> mTransparentObjects{};

};

#endif //RENDERER_RENDERER_H
