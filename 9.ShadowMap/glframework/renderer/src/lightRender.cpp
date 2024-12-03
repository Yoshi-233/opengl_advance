//
// Created by nio on 24-10-31.
//
#include "renderer.h"
#include "scene.h"
#include "phongMaterial.h"
#include "phongEnvMaterial.h"
#include "phongInstancedMaterial.h"
#include <memory>
#include "../../../application/app/Application.h"

void Renderer::processDirectionLight(const std::shared_ptr<Shader> &shaderPtr,
                                     const std::shared_ptr<DirectionalLight> &directionalLight)
{
        CHECK_POINTER_RETURN_VOID(directionalLight);
        /* 平行光参数 */
        shaderPtr->setVectorFloat("directionalLight.direction",
                                  directionalLight->getDirection());
        shaderPtr->setVectorFloat("directionalLight.color", directionalLight->getColor());
        shaderPtr->setFloat("directionalLight.specularIntensity",
                            directionalLight->getSpecularIntensity());
        shaderPtr->setFloat("directionalLight.intensity",
                            directionalLight->getIntensity());
}

void Renderer::processPointLight(const std::shared_ptr<Shader> &shaderPtr,
                                        const std::shared_ptr<PointLight> &pointLight)
{
        CHECK_POINTER_RETURN_VOID(pointLight);

        shaderPtr->setVectorFloat("pointLight.position",
                                  pointLight->getPosition());
        shaderPtr->setVectorFloat("pointLight.color", pointLight->getColor());
        shaderPtr->setFloat("pointLight.specularIntensity",
                            pointLight->getSpecularIntensity());
        shaderPtr->setFloat("pointLight.kc", pointLight->getKc());
        shaderPtr->setFloat("pointLight.k1", pointLight->getK1());
        shaderPtr->setFloat("pointLight.k2", pointLight->getK2());
}

void Renderer::processPointLight(const std::shared_ptr<Shader> &shaderPtr,
                                        const std::vector<std::shared_ptr<PointLight>> &pointLights)
{
        if (pointLights.empty()) {
                shaderPtr->setFloat("pointLightsFlag", 0);
        } else {
                shaderPtr->setFloat("pointLightsFlag", 1);
        }

        for (int i = 0; i < pointLights.size(); i++) {
                auto baseName = "pointLights[" + std::to_string(i) + "]";

                shaderPtr->setVectorFloat(baseName + ".position",
                                          pointLights[i]->getPosition());
                shaderPtr->setVectorFloat(baseName + ".color", pointLights[i]->getColor());
                shaderPtr->setFloat(baseName + ".specularIntensity",
                                    pointLights[i]->getSpecularIntensity());
                shaderPtr->setFloat(baseName + ".kc", pointLights[i]->getKc());
                shaderPtr->setFloat(baseName + ".k1", pointLights[i]->getK1());
                shaderPtr->setFloat(baseName + ".k2", pointLights[i]->getK2());
        }
}

void Renderer::processSpotLight(const std::shared_ptr<Shader> &shaderPtr,
                                       const std::shared_ptr<SpotLight> &spotLight)
{
        if (spotLight == nullptr) {
                shaderPtr->setFloat("spotLightFlag", 0);
                return;
        } else {
                shaderPtr->setFloat("spotLightFlag", 1);
        }
        CHECK_POINTER_RETURN_VOID(spotLight);
        shaderPtr->setVectorFloat("spotLight.position", spotLight->getPosition());
        shaderPtr->setVectorFloat("spotLight.color", spotLight->getColor());
        shaderPtr->setFloat("spotLight.specularIntensity",
                            spotLight->getSpecularIntensity());
        shaderPtr->setVectorFloat("spotLight.targetDirection",
                                  spotLight->getDirection());
        // 下面是弧度值
        shaderPtr->setFloat("spotLight.innerLine",
                            glm::cos(glm::radians(spotLight->getInnerAngle())));
        shaderPtr->setFloat("spotLight.outerLine",
                            glm::cos(glm::radians(spotLight->getOuterAngle())));
}

void Renderer::processSpotLight(const std::shared_ptr<Shader> &shaderPtr,
                                       const std::vector<std::shared_ptr<SpotLight>> &spotLights)
{
        if (spotLights.empty()) {
                shaderPtr->setFloat("spotLightsFlag", 0);
        } else {
                shaderPtr->setFloat("spotLightsFlag", 1);
        }

        for (int i = 0; i < spotLights.size(); i++) {
                auto baseName = "spotLights[" + std::to_string(i) + "]";

                shaderPtr->setVectorFloat(baseName + ".position", spotLights[i]->getPosition());
                shaderPtr->setVectorFloat(baseName + ".color", spotLights[i]->getColor());
                shaderPtr->setFloat(baseName + ".specularIntensity",
                                    spotLights[i]->getSpecularIntensity());
                shaderPtr->setVectorFloat(baseName + ".targetDirection",
                                          spotLights[i]->getDirection());
                // 下面是弧度值
                shaderPtr->setFloat(baseName + ".innerLine",
                                    glm::cos(glm::radians(spotLights[i]->getInnerAngle())));
                shaderPtr->setFloat(baseName + ".outerLine",
                                    glm::cos(glm::radians(spotLights[i]->getOuterAngle())));
        }
}


// 环境光和基础光照
void Renderer::processAmbientShiness(const std::shared_ptr<Shader> &shaderPtr,
                                     const std::shared_ptr<AmbientLight> &ambientLight)
{
        shaderPtr->setVectorFloat("ambientColor", ambientLight->getColor());

}
