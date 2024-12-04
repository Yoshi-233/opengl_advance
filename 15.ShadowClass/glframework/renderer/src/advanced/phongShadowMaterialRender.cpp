//
// Created by nio on 24-11-1.
//
#include "renderer.h"
#include "phongParallaxMaterial.h"
#include "phongShadowMaterial.h"
#include "instanceMesh.h"
#include <memory>
#include "../../../../application/app/Application.h"
#include "../../light/shadow/include/directionalLightShadow.h"

void Renderer::phongShadowMaterialRender(const std::shared_ptr<Shader> &shaderPtr,
                                           const std::shared_ptr<Material> &material,
                                           const std::shared_ptr<Camera> &camera,
                                           const std::shared_ptr<Mesh> &mesh,
                                           const std::shared_ptr<DirectionalLight> &directionalLight,
                                           const std::vector<std::shared_ptr<PointLight>> &pointLights,
                                           const std::shared_ptr<SpotLight> &spotLight,
                                           const std::shared_ptr<AmbientLight> &ambientLight)
{
        auto phongMaterial = (PhongShadowMaterial *) material.get();
        auto dirShadow = std::dynamic_pointer_cast<DirectionalLightShadow>(directionalLight->mShadow);
        // diffuse贴图
        // 设置sampler采样第0号纹理，注意这里默认是0
        // 图片
        CHECK_POINTER_RETURN_VOID(phongMaterial->getDiffuse());
        shaderPtr->setInt("sampler", 0);
        phongMaterial->getDiffuse()->bind();

        /* 高光贴图 */

        /* 法线贴图 */

        /* shadowMap */
        shaderPtr->setInt("shadowMapSampler", 2);
        dirShadow->mRenderTarget->mDepthAttachment->setUnit(2);
        dirShadow->mRenderTarget->mDepthAttachment->bind();
        auto lightMatrix = dirShadow->getLightMatrix(directionalLight->getModelMatrix());
        shaderPtr->setMatrix<decltype(lightMatrix)>("lightMatrix", lightMatrix);

        /* 透明度 */
        shaderPtr->setFloat("opacity", phongMaterial->mOpacity);

        Renderer::processSpecularMask<PhongShadowMaterial>(shaderPtr, phongMaterial);

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

        /* shadowMap bias */
        shaderPtr->setFloat("bias", dirShadow->mBias);

        /* DiskThickness */
        shaderPtr->setFloat("diskTightness", dirShadow->mDiskTightness);

        /* PcfRadius */
        shaderPtr->setFloat("pcfRadius", dirShadow->mPcfRadius);
}