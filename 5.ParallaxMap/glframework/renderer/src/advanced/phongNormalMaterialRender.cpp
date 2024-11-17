//
// Created by nio on 24-11-1.
//
#include "renderer.h"
#include "phongNormalMaterial.h"
#include "instanceMesh.h"
#include <memory>
#include "../../../../application/app/Application.h"

void Renderer::phongNormalMaterialRender(const std::shared_ptr<Shader> &shaderPtr,
                                         const std::shared_ptr<Material> &material,
                                         const std::shared_ptr<Camera> &camera,
                                         const std::shared_ptr<Mesh> &mesh,
                                         const std::shared_ptr<DirectionalLight> &directionalLight,
                                         const std::vector<std::shared_ptr<PointLight>> &pointLights,
                                         const std::shared_ptr<SpotLight> &spotLight,
                                         const std::shared_ptr<AmbientLight> &ambientLight)
{
        auto phongMaterial = (PhongNormalMaterial *) material.get();
        // diffuse贴图
        // 设置sampler采样第0号纹理，注意这里默认是0
        // 图片
        CHECK_POINTER_RETURN_VOID(phongMaterial->getDiffuse());
        shaderPtr->setInt("sampler", 0);
        phongMaterial->getDiffuse()->bind();

        /* 法线贴图 */
        shaderPtr->setInt("normalMapSampler", 2);
        phongMaterial->getNormalMap()->bind();

        /* 透明度 */
        shaderPtr->setFloat("opacity", phongMaterial->mOpacity);

        Renderer::processSpecularMask<PhongNormalMaterial>(shaderPtr, phongMaterial);

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