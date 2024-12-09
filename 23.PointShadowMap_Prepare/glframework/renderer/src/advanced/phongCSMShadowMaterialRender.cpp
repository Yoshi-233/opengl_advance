//
// Created by nio on 24-11-1.
//
#include "renderer.h"
#include "phongCSMShadowMaterial.h"
#include "instanceMesh.h"
#include <memory>
#include "../../../../application/app/Application.h"
#include "../../light/shadow/include/directionalLightCSMShadow.h"
#include "../../../../application/camera/include/orthographicCamera.h"

void Renderer::phongCSMShadowRender(const std::shared_ptr<Shader> &shaderPtr,
                                    const std::shared_ptr<Material> &material,
                                    std::shared_ptr<Camera> &camera,
                                    const std::shared_ptr<Mesh> &mesh,
                                    const std::shared_ptr<DirectionalLight> &directionalLight)
{
        auto dirCSMShadow = std::dynamic_pointer_cast<DirectionalLightCSMShadow>(directionalLight->mShadow);
        /* csm */
        shaderPtr->setInt("csmLayerCount", dirCSMShadow->mLayerCount);
        std::vector<float> layers{};
        dirCSMShadow->generateCascadeLayers(layers, camera->mNear, camera->mFar);
        shaderPtr->setVectorFloat("csmLayers", layers.data(), (int) layers.size());

        /* shadowMap */
        shaderPtr->setInt("shadowMapSampler", 2);
        dirCSMShadow->mRenderTarget->mDepthAttachment->setUnit(2);
        dirCSMShadow->mRenderTarget->mDepthAttachment->bind();
        auto lightMatrices = DirectionalLightCSMShadow::getLightMatrices(camera,
                                                                         directionalLight->getDirection(),
                                                                         layers);
        shaderPtr->setMatrixArray("lightMatrices", lightMatrices.data(), (int) lightMatrices.size());

        /* shadowMap bias */
        shaderPtr->setFloat("bias", dirCSMShadow->mBias);

        /* DiskThickness */
        shaderPtr->setFloat("diskTightness", dirCSMShadow->mDiskTightness);

        /* PcfRadius */
        shaderPtr->setFloat("pcfRadius", dirCSMShadow->mPcfRadius);
}

void Renderer::phongCSMShadowMaterialRender(const std::shared_ptr<Shader> &shaderPtr,
                                            const std::shared_ptr<Material> &material,
                                            std::shared_ptr<Camera> &camera,
                                            const std::shared_ptr<Mesh> &mesh,
                                            const std::shared_ptr<DirectionalLight> &directionalLight,
                                            const std::vector<std::shared_ptr<PointLight>> &pointLights,
                                            const std::shared_ptr<SpotLight> &spotLight,
                                            const std::shared_ptr<AmbientLight> &ambientLight)
{
        auto phongMaterial = (PhongCSMShadowMaterial *) material.get();

        /* diffuse贴图 */
        CHECK_POINTER_RETURN_VOID(phongMaterial->getDiffuse());
        shaderPtr->setInt("sampler", 0);
        phongMaterial->getDiffuse()->bind();

        /* 高光贴图 */

        /* 法线贴图 */
        Renderer::phongCSMShadowRender(shaderPtr, material, camera, mesh, directionalLight);

        Renderer::processSpecularMask<PhongCSMShadowMaterial>(shaderPtr, phongMaterial);

        Renderer::processCommonInfo(shaderPtr, camera, mesh);

        /* 光源参数更新 */
        if (ambientLight != nullptr) {
                Renderer::processAmbientShiness(shaderPtr, ambientLight);
        }
        shaderPtr->setFloat("shiness", phongMaterial->getShiness());


        /* 点光源参数 */
        Renderer::processPointLight(shaderPtr, pointLights);

        /* 平行光参数 */
        if (directionalLight != nullptr) {
                Renderer::processDirectionLight(shaderPtr, directionalLight);
        }

        /* 聚光灯参数 */
        Renderer::processSpotLight(shaderPtr, spotLight);

        // /* shadowMap bias */
        // shaderPtr->setFloat("bias", dirShadow->mBias);
        //
        // /* DiskThickness */
        // shaderPtr->setFloat("diskTightness", dirShadow->mDiskTightness);
        //
        // /* PcfRadius */
        // shaderPtr->setFloat("pcfRadius", dirShadow->mPcfRadius);
        //
        // /* lightSize */
        // shaderPtr->setFloat("lightSize", dirShadow->mLightSize);

        /* frustum && nearPlane */
        // auto shadowCamera = std::dynamic_pointer_cast<OrthographicCamera>(dirShadow->mCamera);
        // float frustum = shadowCamera->mR - shadowCamera->mL;
        // shaderPtr->setFloat("frustum", frustum);
        // shaderPtr->setFloat("nearPlane", shadowCamera->mNear);
}