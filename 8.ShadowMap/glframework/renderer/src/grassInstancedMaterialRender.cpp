//
// Created by nio on 24-11-1.
//
#include "renderer.h"
#include "scene.h"
#include "phongEnvMaterial.h"
#include "instanceMesh.h"
#include <memory>
#include "../../../application/app/Application.h"

void Renderer::grassInstancedMaterialRender(const std::shared_ptr<Shader> &shaderPtr,
                                            const std::shared_ptr<Material> &material,
                                            const std::shared_ptr<Camera> &camera,
                                            const std::shared_ptr<Mesh> &mesh,
                                            const std::shared_ptr<DirectionalLight> &directionalLight,
                                            const std::vector<std::shared_ptr<PointLight>> &pointLights,
                                            const std::shared_ptr<SpotLight> &spotLight,
                                            const std::shared_ptr<AmbientLight> &ambientLight)
{
        auto phongMaterial = (GrassInstancedMaterial *) material.get();
        auto instanceMesh = (InstanceMesh *) mesh.get();
        // instanceMesh->sortMatrices(camera->getViewMatrix());
        instanceMesh->updataMatrices();
        // diffuse贴图
        // 设置sampler采样第0号纹理，注意这里默认是0
        // 图片
        CHECK_POINTER_RETURN_VOID(phongMaterial->getDiffuse());
        shaderPtr->setInt("sampler", 0);
        phongMaterial->getDiffuse()->bind();

        /* 透明度 */
        shaderPtr->setFloat("opacity", phongMaterial->mOpacity);
        shaderPtr->setInt("opacityMask", 2);
        phongMaterial->getOpacityMask()->bind();

        // Renderer::processSpecularMask(shaderPtr, phongMaterial);

        Renderer::processCommonInfo(shaderPtr, camera, mesh);

        /* 光源参数更新 */
        if (ambientLight != nullptr) {
                Renderer::processAmbientShiness(shaderPtr, ambientLight);
        }
        shaderPtr->setFloat("shiness", phongMaterial->getShiness());

        /* 贴图 */
        shaderPtr->setFloat("uvScale", phongMaterial->getUVScale());
        shaderPtr->setFloat("brightness", phongMaterial->mBrightness);
        shaderPtr->setFloat("time", (float) glfwGetTime());

        /* 风力 */
        shaderPtr->setFloat("windStrength", phongMaterial->mWindStrength);
        shaderPtr->setVectorFloat("windDirection", phongMaterial->mWindDirection);
        shaderPtr->setFloat("phaseScale", phongMaterial->mPhaseScale);

        /* 云层 */
        shaderPtr->setInt("cloudMask", 3);
        phongMaterial->mCloudMask->bind();
        shaderPtr->setVectorFloat("cloudWhiteColor", phongMaterial->mCloudWhiteColor);
        shaderPtr->setVectorFloat("cloudBlackColor", phongMaterial->mCloudBlackColor);
        shaderPtr->setFloat("cloudUVScale", phongMaterial->mCloudUVScale);
        shaderPtr->setFloat("cloudSpeed", phongMaterial->mCloudSpeed);
        shaderPtr->setFloat("cloudLerp", phongMaterial->mCloudLerp);

        /* 点光源参数 */
        Renderer::processPointLight(shaderPtr, pointLights);

        /* 平行光参数 */
        if (directionalLight != nullptr) {
                Renderer::processDirectionLight(shaderPtr, directionalLight);
        }

        /* 聚光灯参数 */
        Renderer::processSpotLight(shaderPtr, spotLight);
}