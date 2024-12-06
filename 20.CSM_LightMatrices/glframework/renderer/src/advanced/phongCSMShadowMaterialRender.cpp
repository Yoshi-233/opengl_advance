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

void Renderer::phongCSMShadowMaterialRender(const std::shared_ptr<Shader> &shaderPtr,
                                           const std::shared_ptr<Material> &material,
                                           const std::shared_ptr<Camera> &camera,
                                           const std::shared_ptr<Mesh> &mesh,
                                           const std::shared_ptr<DirectionalLight> &directionalLight,
                                           const std::vector<std::shared_ptr<PointLight>> &pointLights,
                                           const std::shared_ptr<SpotLight> &spotLight,
                                           const std::shared_ptr<AmbientLight> &ambientLight)
{
        auto phongMaterial = (PhongCSMShadowMaterial *) material.get();
        auto dirCSMShadow = std::dynamic_pointer_cast<DirectionalLightCSMShadow>(directionalLight->mShadow);

        /* diffuse贴图 */
        CHECK_POINTER_RETURN_VOID(phongMaterial->getDiffuse());
        shaderPtr->setInt("sampler", 0);
        phongMaterial->getDiffuse()->bind();

        /* 高光贴图 */

        /* 法线贴图 */

        /* shadowMap */
        shaderPtr->setInt("csmLayerCount", dirCSMShadow->mLayerCount);
        std::vector<float> layers;
        dirCSMShadow->generateCascadeLayers(layers,
                                            dirCSMShadow->mCamera->mNear,
                                            dirCSMShadow->mCamera->mFar);
        shaderPtr->setVectorFloat("csmLayers", layers.data(), (int) layers.size());
        // shaderPtr->setInt("shadowMapSampler", 2);
        // dirShadow->mRenderTarget->mDepthAttachment->setUnit(2);
        // dirShadow->mRenderTarget->mDepthAttachment->bind();
        // auto lightMatrix = dirShadow->getLightMatrix(directionalLight->getModelMatrix());
        // shaderPtr->setMatrix<decltype(lightMatrix)>("lightMatrix", lightMatrix);
        // shaderPtr->setMatrix<decltype(glm::inverse(directionalLight->getModelMatrix()))>("lightViewMatrix",
        //                                                           glm::inverse(directionalLight->getModelMatrix()));

        /* 透明度 */
        shaderPtr->setFloat("opacity", phongMaterial->mOpacity);

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