//
// Created by nio on 24-11-1.
//
#include "renderer.h"
#include "phongParallaxMaterial.h"
#include "phongPointShadowMaterial.h"
#include "phongShadowMaterial.h"
#include "instanceMesh.h"
#include <memory>
#include "../../../../application/app/Application.h"
#include "../../light/shadow/include/pointLightShadow.h"
#include "../../light/shadow/include/directionalLightShadow.h"
#include "../../light/shadow/include/directionalLightCSMShadow.h"
#include "../../../../application/camera/include/orthographicCamera.h"

void Renderer::phongPointShadowMaterialRender(const std::shared_ptr<Shader> &shaderPtr,
                                              const std::shared_ptr<Material> &material,
                                              std::shared_ptr<Camera> &camera,
                                              const std::shared_ptr<Mesh> &mesh,
                                              const std::shared_ptr<DirectionalLight> &directionalLight,
                                              const std::vector<std::shared_ptr<PointLight>> &pointLights,
                                              const std::shared_ptr<SpotLight> &spotLight,
                                              const std::shared_ptr<AmbientLight> &ambientLight)
{
        auto phongMaterial = (PhongPointShadowMaterial *) material.get();

        /* diffuse贴图 */
        CHECK_POINTER_RETURN_VOID(phongMaterial->getDiffuse());
        shaderPtr->setInt("sampler", 0);
        phongMaterial->getDiffuse()->bind();

        /* 高光贴图 */
        Renderer::processSpecularMask<PhongPointShadowMaterial>(shaderPtr, phongMaterial);
        /* 法线贴图 */

        /* shadowMap */
        if (directionalLight != nullptr) {
                if (directionalLight->mUseCSM) {
                        Renderer::phongCSMShadowRender(shaderPtr, material, camera, mesh, directionalLight);
                } else {
                        Renderer::phongDirectionalLightShadowRender(shaderPtr, material, mesh, directionalLight);
                }
        }

        for (const auto &pointLight: pointLights) {

        }

        /* 透明度 */
        shaderPtr->setFloat("opacity", phongMaterial->mOpacity);

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
}