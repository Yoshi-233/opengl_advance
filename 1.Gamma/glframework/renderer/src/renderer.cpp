//
// Created by Shaozheming on 2024/9/19.
//

#include "renderer.h"
#include "scene.h"
#include "phongMaterial.h"
#include "phongEnvMaterial.h"
#include "phongInstancedMaterial.h"
#include "opacityMaskMaterial.h"
#include "screenMaterial.h"
#include "cubeMaterial.h"
#include "instanceMesh.h"
#include <memory>
#include <algorithm>
#include "../../../application/app/Application.h"

Renderer::Renderer()
{
        auto phongVertexPath = std::string(PROJECT_DIR) + "/assets/shaders/phong.vert";
        auto phongFragmentPath = std::string(PROJECT_DIR) + "/assets/shaders/phong.frag";
        this->mPhoneShader = std::make_shared<Shader>(phongVertexPath.c_str(),
                                                      phongFragmentPath.c_str());

        auto phongEnvVertexPath = std::string(PROJECT_DIR) + "/assets/shaders/phongEnv.vert";
        auto phongEnvFragmentPath = std::string(PROJECT_DIR) + "/assets/shaders/phongEnv.frag";
        this->mPhoneEnvShader = std::make_shared<Shader>(phongEnvVertexPath.c_str(),
                                                         phongEnvFragmentPath.c_str());

        auto phongInstancedVertexPath = std::string(PROJECT_DIR) + "/assets/shaders/phongInstanced.vert";
        auto phongInstancedFragmentPath = std::string(PROJECT_DIR) + "/assets/shaders/phongInstanced.frag";
        this->mPhoneInstancedShader = std::make_shared<Shader>(phongInstancedVertexPath.c_str(),
                                                               phongInstancedFragmentPath.c_str());

        auto grassInstancedVertexPath = std::string(PROJECT_DIR) + "/assets/shaders/grassInstanced.vert";
        auto grassInstancedFragmentPath = std::string(PROJECT_DIR) + "/assets/shaders/grassInstanced.frag";
        this->mGrassInstancedShader = std::make_shared<Shader>(grassInstancedVertexPath.c_str(),
                                                               grassInstancedFragmentPath.c_str());

        auto whiteVertexPath = std::string(PROJECT_DIR) + "/assets/shaders/white.vert";
        auto whiteFragmentPath = std::string(PROJECT_DIR) + "/assets/shaders/white.frag";
        this->mWhiteShader = std::make_shared<Shader>(whiteVertexPath.c_str(),
                                                      whiteFragmentPath.c_str());

        auto depthVertexPath = std::string(PROJECT_DIR) + "/assets/shaders/depth.vert";
        auto depthFragmentPath = std::string(PROJECT_DIR) + "/assets/shaders/depth.frag";
        this->mDepthShader = std::make_shared<Shader>(depthVertexPath.c_str(),
                                                      depthFragmentPath.c_str());

        auto opacityMaskVertexPath = std::string(PROJECT_DIR) + "/assets/shaders/phongOpacityMask.vert";
        auto opacityMaskFragmentPath = std::string(PROJECT_DIR) + "/assets/shaders/phongOpacityMask.frag";
        this->mOpacityMaskShader = std::make_shared<Shader>(opacityMaskVertexPath.c_str(),
                                                            opacityMaskFragmentPath.c_str());

        auto screenVertexPath = std::string(PROJECT_DIR) + "/assets/shaders/screen.vert";
        auto screenFragmentPath = std::string(PROJECT_DIR) + "/assets/shaders/screen.frag";
        this->mScreenShader = std::make_shared<Shader>(screenVertexPath.c_str(),
                                                       screenFragmentPath.c_str());

        auto cubeVertexPath = std::string(PROJECT_DIR) + "/assets/shaders/cube.vert";
        auto cubeFragmentPath = std::string(PROJECT_DIR) + "/assets/shaders/cube.frag";
        this->mCubeMapShader = std::make_shared<Shader>(cubeVertexPath.c_str(),
                                                        cubeFragmentPath.c_str());

}

Renderer::~Renderer()
= default;

std::shared_ptr<Shader> Renderer::pickShader(MaterialType type)
{
        std::shared_ptr<Shader> result = nullptr;

        switch (type) {
                case MaterialType::PhongMaterial:
                        result = this->mPhoneShader;
                        break;
                case MaterialType::PhongEnvMaterial:
                        result = this->mPhoneEnvShader;
                        break;
                case MaterialType::PhongInstancedMaterial:
                        result = this->mPhoneInstancedShader;
                        break;
                case MaterialType::GrassInstancedMaterial:
                        result = this->mGrassInstancedShader;
                        break;
                case MaterialType::WhiteMaterial:
                        result = this->mWhiteShader;
                        break;
                case MaterialType::DepthMaterial:
                        result = this->mDepthShader;
                        break;
                case MaterialType::OpacityMaskMaterial:
                        result = this->mOpacityMaskShader;
                        break;
                case MaterialType::ScreenMaterial:
                        result = this->mScreenShader;
                        break;
                case MaterialType::CubeMaterial:
                        result = this->mCubeMapShader;
                        break;
                default:
                        ERROR("Unsupported material type: {}", static_cast<int>(type));
                        break;
        }

        return result;
}

void Renderer::projectObject(const std::shared_ptr<Object> &object)
{
        if (object->getType() == ObjectType::MESH ||
            object->getType() == ObjectType::INSTANCED_MESH) {
                auto mesh = std::dynamic_pointer_cast<Mesh>(object);
                auto material = mesh->getMaterial();
                if (material->mBlend) {
                        this->mTransparentObjects.push_back(mesh);
                } else {
                        this->mOpacityObjects.push_back(mesh);
                }
        }

        auto childrens = object->getChildren();
        for (const auto &child: childrens) {
                this->projectObject(child);
        }
}

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

inline void Renderer::processPointLight(const std::shared_ptr<Shader> &shaderPtr,
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

inline void Renderer::processPointLight(const std::shared_ptr<Shader> &shaderPtr,
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

inline void Renderer::processSpotLight(const std::shared_ptr<Shader> &shaderPtr,
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
                                  spotLight->getTargetDirection());
        // 下面是弧度值
        shaderPtr->setFloat("spotLight.innerLine",
                            glm::cos(glm::radians(spotLight->getInnerAngle())));
        shaderPtr->setFloat("spotLight.outerLine",
                            glm::cos(glm::radians(spotLight->getOuterAngle())));
}

inline void Renderer::processSpotLight(const std::shared_ptr<Shader> &shaderPtr,
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
                                          spotLights[i]->getTargetDirection());
                // 下面是弧度值
                shaderPtr->setFloat(baseName + ".innerLine",
                                    glm::cos(glm::radians(spotLights[i]->getInnerAngle())));
                shaderPtr->setFloat(baseName + ".outerLine",
                                    glm::cos(glm::radians(spotLights[i]->getOuterAngle())));
        }
}

void Renderer::processSpecularMask(const std::shared_ptr<Shader> &shaderPtr,
                                   PhongMaterial *phongMaterial)
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

void Renderer::processSpecularMask(const std::shared_ptr<Shader> &shaderPtr,
                                   PhongEnvMaterial *phongMaterial)
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

void Renderer::processSpecularMask(const std::shared_ptr<Shader> &shaderPtr,
                                   PhongInstancedMaterial *phongMaterial)
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

void Renderer::processSpecularMask(const std::shared_ptr<Shader> &shaderPtr,
                                   GrassInstancedMaterial *phongMaterial)
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

void Renderer::processCommonInfo(const std::shared_ptr<Shader> &shaderPtr,
                                 const std::shared_ptr<Camera> &camera,
                                 const std::shared_ptr<Mesh> &mesh)
{
        shaderPtr->setMatrix<decltype(mesh->getModelMatrix())>("modelMatrix",
                                                               mesh->getModelMatrix());
        shaderPtr->setMatrix<decltype(camera->getViewMatrix())>("viewMatrix",
                                                                camera->getViewMatrix());
        shaderPtr->setMatrix<decltype(camera->getProjectionMatrix())>("projectionMatrix",
                                                                      camera->getProjectionMatrix());

        auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
        shaderPtr->setMatrix<decltype(normalMatrix)>("normalMatrix", normalMatrix);

        // 光源参数更新
        shaderPtr->setVectorFloat("cameraPosition", camera->mPosition);
        shaderPtr->setFloat("near", camera->mNear);
        shaderPtr->setFloat("far", camera->mFar);
}

// 环境光和基础光照
void Renderer::processAmbientShiness(const std::shared_ptr<Shader> &shaderPtr,
                                     const std::shared_ptr<AmbientLight> &ambientLight)
{
        shaderPtr->setVectorFloat("ambientColor", ambientLight->getColor());

}

void Renderer::processWhiteMaterial(const std::shared_ptr<Shader> &shaderPtr,
                                    const std::shared_ptr<Mesh> &mesh,
                                    const std::shared_ptr<Camera> &camera)
{
        shaderPtr->setMatrix<decltype(mesh->getModelMatrix())>("modelMatrix",
                                                               mesh->getModelMatrix());
        shaderPtr->setMatrix<decltype(camera->getViewMatrix())>("viewMatrix",
                                                                camera->getViewMatrix());
        shaderPtr->setMatrix<decltype(camera->getProjectionMatrix())>("projectionMatrix",
                                                                      camera->getProjectionMatrix());
}

void Renderer::processDepthMaterial(const std::shared_ptr<Shader> &shaderPtr,
                                    const std::shared_ptr<Mesh> &mesh,
                                    const std::shared_ptr<Camera> &camera)
{
        shaderPtr->setMatrix<decltype(mesh->getModelMatrix())>("modelMatrix",
                                                               mesh->getModelMatrix());
        shaderPtr->setMatrix<decltype(camera->getViewMatrix())>("viewMatrix",
                                                                camera->getViewMatrix());
        shaderPtr->setMatrix<decltype(camera->getProjectionMatrix())>("projectionMatrix",
                                                                      camera->getProjectionMatrix());

        shaderPtr->setFloat("near", camera->mNear);
        shaderPtr->setFloat("far", camera->mFar);
}

void Renderer::processCubeMaterial(const std::shared_ptr<Shader> &shaderPtr,
                                   const std::shared_ptr<Mesh> &mesh,
                                   const std::shared_ptr<Camera> &camera)
{
        shaderPtr->setMatrix<decltype(mesh->getModelMatrix())>("modelMatrix",
                                                               mesh->getModelMatrix());
        shaderPtr->setMatrix<decltype(camera->getViewMatrix())>("viewMatrix",
                                                                camera->getViewMatrix());
        shaderPtr->setMatrix<decltype(camera->getProjectionMatrix())>("projectionMatrix",
                                                                      camera->getProjectionMatrix());
}

void Renderer::renderObject(const std::shared_ptr<Object> &object, const std::shared_ptr<Camera> &camera,
                            const std::shared_ptr<DirectionalLight> &directionalLight,
                            const std::vector<std::shared_ptr<PointLight>> &pointLights,
                            const std::shared_ptr<SpotLight> &spotLight,
                            const std::shared_ptr<AmbientLight> &ambientLight)
{
        if (object->getType() == ObjectType::MESH ||
            object->getType() == ObjectType::INSTANCED_MESH) {
                auto mesh = std::dynamic_pointer_cast<Mesh>(object);
                // 检查是否拥有全局材质
                auto material = (this->mGlobalMaterial != nullptr) ? this->mGlobalMaterial : mesh->getMaterial();
                auto geometry = mesh->getGeometry();

                // 设置渲染状态
                Renderer::setGLStatus(material);

                /* 3.1 决定使用哪个shader */
                std::shared_ptr<Shader> shaderPtr = this->pickShader(material->mType);

                /* 3.2 设置shader的uniform变量 */
                shaderPtr->begin();
                // 纹理和纹理单元匹配
                switch (material->mType) {
                        case MaterialType::PhongMaterial: {
                                auto phongMaterial = (PhongMaterial *) material.get();
                                // diffuse贴图
                                // 设置sampler采样第0号纹理，注意这里默认是0
                                // 图片
                                CHECK_POINTER_RETURN_VOID(phongMaterial->getDiffuse());
                                shaderPtr->setInt("sampler", 0);
                                phongMaterial->getDiffuse()->bind();

                                /* 透明度 */
                                shaderPtr->setFloat("opacity", phongMaterial->mOpacity);

                                Renderer::processSpecularMask(shaderPtr, phongMaterial);

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
                                break;
                        case MaterialType::PhongEnvMaterial: {
                                auto phongMaterial = (PhongEnvMaterial *) material.get();
                                // diffuse贴图
                                // 设置sampler采样第0号纹理，注意这里默认是0
                                // 图片
                                CHECK_POINTER_RETURN_VOID(phongMaterial->getDiffuse());
                                shaderPtr->setInt("sampler", 0);
                                phongMaterial->getDiffuse()->bind();

                                /* 透明度 */
                                shaderPtr->setFloat("opacity", phongMaterial->mOpacity);

                                Renderer::processSpecularMask(shaderPtr, phongMaterial);

                                if (phongMaterial->getEnv() != nullptr) {
                                        shaderPtr->setInt("envSampler", 2);
                                        phongMaterial->getEnv()->bind();
                                }

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
                                break;
                        case MaterialType::PhongInstancedMaterial: {
                                auto phongMaterial = (PhongInstancedMaterial *) material.get();
                                auto instanceMesh = (InstanceMesh *) mesh.get();
                                // diffuse贴图
                                // 设置sampler采样第0号纹理，注意这里默认是0
                                // 图片
                                CHECK_POINTER_RETURN_VOID(phongMaterial->getDiffuse());
                                shaderPtr->setInt("sampler", 0);
                                phongMaterial->getDiffuse()->bind();

                                /* 透明度 */
                                shaderPtr->setFloat("opacity", phongMaterial->mOpacity);

                                Renderer::processSpecularMask(shaderPtr, phongMaterial);

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
                                break;
                        case MaterialType::GrassInstancedMaterial: {
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
                                break;
                        case MaterialType::OpacityMaskMaterial: {
                                auto opacityMaterial = (OpacityMaskMaterial *) material.get();
                                // diffuse贴图
                                // 设置sampler采样第0号纹理，注意这里默认是0
                                // 图片
                                CHECK_POINTER_RETURN_VOID(opacityMaterial->getDiffuse());
                                shaderPtr->setInt("sampler", 0);
                                opacityMaterial->getDiffuse()->bind();

                                /* 透明度 */
                                shaderPtr->setFloat("opacity", opacityMaterial->mOpacity);
                                /* 透明度蒙板 */
                                if (opacityMaterial->getOpacityMask() != nullptr) {
                                        shaderPtr->setInt("opacityMaskSampler", 1);
                                        opacityMaterial->getOpacityMask()->bind();
                                }

                                Renderer::processCommonInfo(shaderPtr, camera, mesh);

                                /* 光源参数更新 */
                                if (ambientLight != nullptr) {
                                        Renderer::processAmbientShiness(shaderPtr, ambientLight);
                                }
                                shaderPtr->setFloat("shiness", opacityMaterial->getShiness());

                                /* 点光源参数 */
                                Renderer::processPointLight(shaderPtr, pointLights);

                                /* 平行光参数 */
                                if (directionalLight != nullptr) {
                                        Renderer::processDirectionLight(shaderPtr, directionalLight);
                                }

                                /* 聚光灯参数 */
                                Renderer::processSpotLight(shaderPtr, spotLight);
                        }
                                break;
                        case MaterialType::ScreenMaterial: {
                                auto screenMaterial = (ScreenMaterial *) material.get();
                                // diffuse贴图
                                // 设置sampler采样第0号纹理，注意这里默认是0
                                // 图片
                                CHECK_POINTER_RETURN_VOID(screenMaterial->mScreenTexture);
                                shaderPtr->setInt("screenTextureSampler", 0);
                                shaderPtr->setFloat("textureWidth", APP.getWidth());
                                shaderPtr->setFloat("textureHeight", APP.getHeight());
                                screenMaterial->mScreenTexture->bind();
                        }
                                break;
                        case MaterialType::WhiteMaterial: {
                                Renderer::processWhiteMaterial(shaderPtr, mesh, camera);
                        }
                                break;
                        case MaterialType::DepthMaterial: {
                                Renderer::processDepthMaterial(shaderPtr, mesh, camera);
                        }
                                break;
                        case MaterialType::CubeMaterial: {
                                auto cubeMaterial = (CubeMaterial *) material.get();

                                mesh->setPosition(camera->mPosition); //这样mesh跟着相机走
                                Renderer::processCubeMaterial(shaderPtr, mesh, camera);
                                shaderPtr->setInt("sphericalSampler", 0);
                                cubeMaterial->mDiffuse->bind();
                        }
                                break;
                        default:
                                break;
                };

                /* 3.3 绑定vao材质 */
                glBindVertexArray(geometry->getVao());

                /* 3.4 绘制 */
                /* 第一次绘制 */
                if (object->getType() == ObjectType::INSTANCED_MESH) {
                        auto instanceMesh = (InstanceMesh *) mesh.get();
                        glDrawElementsInstanced(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, nullptr,
                                                (int) instanceMesh->numInstanced);
                } else {
                        glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, nullptr);
                }// 这里最好解绑，这样误操作就不会影响当前vao
                Shader::end();
        }

}

void Renderer::render(const std::shared_ptr<Scene> &scene,
                      const std::shared_ptr<Camera> &camera,
                      const std::shared_ptr<DirectionalLight> &directionalLight,
                      const std::vector<std::shared_ptr<PointLight>> &pointLights,
                      const std::shared_ptr<SpotLight> &spotLight,
                      const std::shared_ptr<AmbientLight> &ambientLight,
                      unsigned int fbo
)
{
        if (scene == nullptr || camera == nullptr) {
                return;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        /* 1.初始化 */
        Renderer::setGLInitStatus(scene, camera);

        /* 2. 渲染两个队列, 注意必须按照顺序渲染 */
        /* 2.1 不透明物体 */
        for (const auto &mesh: this->mOpacityObjects) {
                Renderer::renderObject(mesh, camera, directionalLight, pointLights, spotLight, ambientLight);
        }

        /* 2.2 透明物体 */
        for (const auto &mesh: this->mTransparentObjects) {
                Renderer::renderObject(mesh, camera, directionalLight, pointLights, spotLight, ambientLight);
        }
}

void Renderer::render(const std::shared_ptr<Scene> &scene, const std::shared_ptr<Camera> &camera,
                      const std::shared_ptr<DirectionalLight> &directionalLight,
                      const std::shared_ptr<AmbientLight> &ambientLight,
                      unsigned int fbo)
{
        if (scene == nullptr || camera == nullptr) {
                return;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        Renderer::setGLInitStatus(scene, camera);

        auto pointLights = std::vector<std::shared_ptr<PointLight>>{};
        /* 2. 渲染两个队列 */
        /* 2.1 不透明物体 */
        for (const auto &mesh: this->mOpacityObjects) {
                Renderer::renderObject(mesh, camera, directionalLight, pointLights, nullptr, ambientLight);
        }

        /* 2.2 透明物体 */
        for (const auto &mesh: this->mTransparentObjects) {
                Renderer::renderObject(mesh, camera, directionalLight, pointLights, nullptr, ambientLight);
        }
}

void Renderer::render(const std::shared_ptr<Scene> &scene,
                      const std::shared_ptr<Camera> &camera,
                      const std::shared_ptr<PointLight> &pointLight,
                      const std::shared_ptr<AmbientLight> &ambientLight,
                      unsigned int fbo)
{
        if (scene == nullptr || camera == nullptr) {
                return;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        Renderer::setGLInitStatus(scene, camera);

        auto pointLights = std::vector<std::shared_ptr<PointLight>>{};
        pointLights.push_back(pointLight);
        /* 2. 渲染两个队列 */
        /* 2.1 不透明物体 */
        for (const auto &mesh: this->mOpacityObjects) {
                Renderer::renderObject(mesh, camera, nullptr, pointLights, nullptr, ambientLight);
        }

        /* 2.2 透明物体 */
        for (const auto &mesh: this->mTransparentObjects) {
                Renderer::renderObject(mesh, camera, nullptr, pointLights, nullptr, ambientLight);
        }
}

void Renderer::setClearColor(const glm::vec3 &color)
{
        glClearColor(color.r, color.g, color.b, 1.0f);
}

void Renderer::setGLInitStatus(const std::shared_ptr<Scene> &scene,
                               const std::shared_ptr<Camera> &camera)
{
        /* 1. opengl的必要状态机, 为默认设置，否则，当最后一个mesh没有写入时，会导致glClear失败 */
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);
        glDisable(GL_POLYGON_OFFSET_FILL);
        glDisable(GL_POLYGON_OFFSET_LINE);

        /* 打开模板测试写入 */
        glEnable(GL_STENCIL_TEST);
        // 初始状态下，模板测试通过，模板测试通过但深度测试未通过，模板深度测试都通过，保持不变
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        glStencilMask(0xFF); // 开启模板测试写入, 0xFF表示全部写入也保证了模板缓冲可以被清理, 0x00表示全部不写入

        /* 默认关闭颜色混合 */
        glDisable(GL_BLEND);

        /* 2.清理画布 */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        /* 3.清空透明不透明队列 */
        this->mTransparentObjects.clear();
        this->mOpacityObjects.clear();

        this->projectObject(std::static_pointer_cast<Object>(scene));

        // 排序
        std::sort(
                this->mTransparentObjects.begin(),
                this->mTransparentObjects.end(),
                [camera](const std::shared_ptr<Mesh> &m1, const std::shared_ptr<Mesh> &m2) {
                        // 1.计算相机坐标系，此时这里的坐标系是相对父亲节点的局部坐标系
                        auto viewMatrix = camera->getViewMatrix();
                        // auto posM1 = m1->getPosition();
                        auto modelMatrixM1 = m1->getModelMatrix();
                        auto worldPosM1 = modelMatrixM1 * glm::vec4(0, 0, 0, 1);
                        auto cameraPosM1 = viewMatrix * worldPosM1; // 相机坐标系

                        // auto posM2 = m2->getPosition();
                        auto modelMatrixM2 = m2->getModelMatrix();
                        auto worldPosM2 = modelMatrixM2 * glm::vec4(0, 0, 0, 1);
                        auto cameraPosM2 = viewMatrix * worldPosM2; // 相机坐标系

                        // 2.根据距离排序, 升序
                        return cameraPosM1.z < cameraPosM2.z;
                });
}

void Renderer::setGLStatus(const std::shared_ptr<Material> &material)
{
        Renderer::setDepthStatus(material);
        /* PolygonOffset */
        Renderer::setPolygonOffsetStatus(material);
        /* 模板测试 */
        Renderer::setStencilStatus(material);
        /* 混合 */
        Renderer::setBlendStatus(material);
        /* 面剔除 */
        Renderer::setFaceCullingStatus(material);

}

void Renderer::setDepthStatus(const std::shared_ptr<Material> &material)
{
        if (material->mDepthTest) {
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(material->mDepthFunc);
        } else {
                glDisable(GL_DEPTH_TEST);
        }

        if (material->mDepthWrite) {
                glDepthMask(GL_TRUE);
        } else {
                glDepthMask(GL_FALSE);
        }
}

void Renderer::setPolygonOffsetStatus(const std::shared_ptr<Material> &material)
{
        if (material->mPolygonOffset) {
                glEnable(material->mPolygonOffsetType);
                /*
                 * param1 * 斜率因子 + param2 * 最小精度偏移
                 * 1.第一个参数是斜率因子，当两个比较近的mesh同时旋转时，距离会很小，造成穿模的现象
                 * 此时，近平面影响不大，但远平面较为严重，远平面的偏导数较低（比实际图像的df/dx df/dy低）
                 * 因此斜率因子可以根据这一特性使得较远处的平面偏移更大些
                 * 2. 当两个mesh很近时，出现穿模现象，使用该参数可以让平面稍微后移，精度为float
                 * */
                glPolygonOffset(material->mFactor, material->mUnit);
        } else {
                glDisable(GL_POLYGON_OFFSET_FILL);
                glDisable(GL_POLYGON_OFFSET_LINE);
        }
}

void Renderer::setStencilStatus(const std::shared_ptr<Material> &material)
{
        if (material->mStencilTest) {
                glEnable(GL_STENCIL_TEST);
                // 对于本体, 允许模板测试通过
                glStencilFunc(material->mStencilFunc, (GLint) material->mStencilRef, material->mStencilFuncMask);
                glStencilOp(material->mSFail, material->mZFail, material->mZPass); // 都通过后替换为0x1
                glStencilMask(material->mStencilMask); // 这里开启防止前一个关闭了写入
        } else {
                glDisable(GL_STENCIL_TEST);
        }
}

void Renderer::setBlendStatus(const std::shared_ptr<Material> &material)
{
        if (material->mBlend) {
                glEnable(GL_BLEND);
                // src_color * src_alpha + dst_color * (1 - src_alpha)
                glBlendFunc(material->mSFactor, material->mDFactor);
        } else {
                glDisable(GL_BLEND);
        }
}

void Renderer::setFaceCullingStatus(const std::shared_ptr<Material> &material)
{
        if (material->mFaceCulling) {
                glEnable(GL_CULL_FACE);
                glFrontFace(material->mFronFace);
                glCullFace(material->mCullFace);
        } else {
                glDisable(GL_CULL_FACE);
        }
}












