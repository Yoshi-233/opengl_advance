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
        auto vertexPath = std::string(PROJECT_DIR) + "/assets/shaders/advanced/phong.vert";
        auto fragmentPath = std::string(PROJECT_DIR) + "/assets/shaders/advanced/phong.frag";
        this->mPhoneShader = std::make_shared<Shader>(vertexPath.c_str(),
                                                      fragmentPath.c_str());

        vertexPath = std::string(PROJECT_DIR) + "/assets/shaders/phongEnv.vert";
        fragmentPath = std::string(PROJECT_DIR) + "/assets/shaders/phongEnv.frag";
        this->mPhoneEnvShader = std::make_shared<Shader>(vertexPath.c_str(),
                                                         fragmentPath.c_str());

        vertexPath = std::string(PROJECT_DIR) + "/assets/shaders/phongInstanced.vert";
        fragmentPath = std::string(PROJECT_DIR) + "/assets/shaders/phongInstanced.frag";
        this->mPhoneInstancedShader = std::make_shared<Shader>(vertexPath.c_str(),
                                                               fragmentPath.c_str());

        vertexPath = std::string(PROJECT_DIR) + "/assets/shaders/grassInstanced.vert";
        fragmentPath = std::string(PROJECT_DIR) + "/assets/shaders/grassInstanced.frag";
        this->mGrassInstancedShader = std::make_shared<Shader>(vertexPath.c_str(),
                                                               fragmentPath.c_str());

        vertexPath = std::string(PROJECT_DIR) + "/assets/shaders/white.vert";
        fragmentPath = std::string(PROJECT_DIR) + "/assets/shaders/white.frag";
        this->mWhiteShader = std::make_shared<Shader>(vertexPath.c_str(),
                                                      fragmentPath.c_str());

        vertexPath = std::string(PROJECT_DIR) + "/assets/shaders/depth.vert";
        fragmentPath = std::string(PROJECT_DIR) + "/assets/shaders/depth.frag";
        this->mDepthShader = std::make_shared<Shader>(vertexPath.c_str(),
                                                      fragmentPath.c_str());

        vertexPath = std::string(PROJECT_DIR) + "/assets/shaders/phongOpacityMask.vert";
        fragmentPath = std::string(PROJECT_DIR) + "/assets/shaders/phongOpacityMask.frag";
        this->mOpacityMaskShader = std::make_shared<Shader>(vertexPath.c_str(),
                                                            fragmentPath.c_str());

        vertexPath = std::string(PROJECT_DIR) + "/assets/shaders/screen.vert";
        fragmentPath = std::string(PROJECT_DIR) + "/assets/shaders/screen.frag";
        this->mScreenShader = std::make_shared<Shader>(vertexPath.c_str(),
                                                       fragmentPath.c_str());

        vertexPath = std::string(PROJECT_DIR) + "/assets/shaders/cube.vert";
        fragmentPath = std::string(PROJECT_DIR) + "/assets/shaders/cube.frag";
        this->mCubeMapShader = std::make_shared<Shader>(vertexPath.c_str(),
                                                        fragmentPath.c_str());

        vertexPath = std::string(PROJECT_DIR) + "/assets/shaders/advanced/phongNormal.vert";
        fragmentPath = std::string(PROJECT_DIR) + "/assets/shaders/advanced/phongNormal.frag";
        this->mPhongNormalShader = std::make_shared<Shader>(vertexPath.c_str(),
                                                            fragmentPath.c_str());

        vertexPath = std::string(PROJECT_DIR) + "/assets/shaders/advanced/phongParallax.vert";
        fragmentPath = std::string(PROJECT_DIR) + "/assets/shaders/advanced/phongParallax.frag";
        this->mPhongParallaxShader = std::make_shared<Shader>(vertexPath.c_str(),
                                                              fragmentPath.c_str());

        vertexPath = std::string(PROJECT_DIR) + "/assets/shaders/advanced/shadow.vert";
        fragmentPath = std::string(PROJECT_DIR) + "/assets/shaders/advanced/shadow.frag";
        this->mShadowShader = std::make_shared<Shader>(vertexPath.c_str(),
                                                              fragmentPath.c_str());

        vertexPath = std::string(PROJECT_DIR) + "/assets/shaders/advanced/phongShadow.vert";
        fragmentPath = std::string(PROJECT_DIR) + "/assets/shaders/advanced/phongShadow.frag";
        this->mPhongShadowShader = std::make_shared<Shader>(vertexPath.c_str(),
                                                              fragmentPath.c_str());
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
                /* advanced */
                case MaterialType::PhongNormalMaterial:
                        result = this->mPhongNormalShader;
                        break;
                case MaterialType::PhongParallaxMaterial:
                        result = this->mPhongParallaxShader;
                        break;
                case MaterialType::PhongShadowMaterial:
                        result = this->mPhongShadowShader;
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

        /* 渲染shadowmap */
        this->renderShadowMap(this->mOpacityObjects, directionalLight,
                              directionalLight->mShadow->mRenderTarget);

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
                                Renderer::phongMaterialRender(shaderPtr, material, camera, mesh,
                                                              directionalLight, pointLights,
                                                              spotLight, ambientLight);
                        }
                                break;
                        case MaterialType::PhongEnvMaterial: {
                                Renderer::phongEnvMaterialRender(shaderPtr, material, camera, mesh,
                                                                 directionalLight, pointLights,
                                                                 spotLight, ambientLight);
                        }
                                break;
                        case MaterialType::PhongInstancedMaterial: {
                                Renderer::phongInstancedMaterialRender(shaderPtr, material, camera, mesh,
                                                                       directionalLight, pointLights,
                                                                       spotLight, ambientLight);
                        }
                                break;
                        case MaterialType::GrassInstancedMaterial: {
                                Renderer::grassInstancedMaterialRender(shaderPtr, material, camera, mesh,
                                                                       directionalLight, pointLights,
                                                                       spotLight, ambientLight);
                        }
                                break;
                        case MaterialType::OpacityMaskMaterial: {
                                Renderer::opacityMaskMaterialRender(shaderPtr, material, camera, mesh,
                                                                    directionalLight, pointLights,
                                                                    spotLight, ambientLight);
                        }
                                break;
                        case MaterialType::PhongNormalMaterial: {
                                Renderer::phongNormalMaterialRender(shaderPtr, material, camera, mesh,
                                                                    directionalLight, pointLights,
                                                                    spotLight, ambientLight);
                        }
                                break;
                        case MaterialType::PhongParallaxMaterial: {
                                Renderer::phongParallaxMaterialRender(shaderPtr, material, camera, mesh,
                                                                      directionalLight, pointLights,
                                                                      spotLight, ambientLight);
                        }
                                break;
                        case MaterialType::PhongShadowMaterial: {
                                Renderer::phongShadowMaterialRender(shaderPtr, material, camera, mesh,
                                                                      directionalLight, pointLights,
                                                                      spotLight, ambientLight);
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




