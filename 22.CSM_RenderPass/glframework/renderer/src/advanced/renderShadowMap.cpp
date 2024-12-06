//
// Created by nio on 24-12-3.
//

#include "renderer.h"
#include "../../../../application/camera/include/orthographicCamera.h"
#include "../../mesh/include/instanceMesh.h"
#include "../../light/shadow/include/directionalLightShadow.h"
#include "../../light/shadow/include/directionalLightCSMShadow.h"

void Renderer::renderShadowMap(std::shared_ptr<Camera> &camera,
                               const std::vector<std::shared_ptr<Mesh>> &meshes,
                               const std::shared_ptr<DirectionalLight> &directionalLight)
{
        /* 1.做好排除工作，只有offscreen需要渲染，(onscreen)PostProcessPass不需要 */
        bool isPostProcessPass = true;
        for (auto &mesh: meshes) {
                if (mesh->mMaterial->mType != MaterialType::ScreenMaterial) {
                        isPostProcessPass = false;
                        break;
                }
        }

        if (isPostProcessPass) {
                return;
        }

        /* 2.做好备份工作，不能修改正在使用的offscreen fbo和viewport */
        GLint preFbo;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &preFbo); // 获取当前绑定的framebuffer

        GLint preViewport[4];
        glGetIntegerv(GL_VIEWPORT, preViewport); // 获取当前viewport

        /* 3.设置好渲染状态 */
        glEnable(GL_DEPTH_TEST); // 开启深度测试
        glDepthFunc(GL_LESS); // 设置深度测试函数
        glDepthMask(GL_TRUE); // 允许写入深度缓冲

        /* 4.循环每个视锥体，渲染shadow map */
        auto csmShadow = std::dynamic_pointer_cast<DirectionalLightCSMShadow>(directionalLight->mShadow);
        std::vector<float> layers;
        csmShadow->generateCascadeLayers(layers, camera->mNear, camera->mFar);
        auto lightMatrices = DirectionalLightCSMShadow::getLightMatrices(camera, directionalLight->getDirection(),
                                                                         layers);

        glBindFramebuffer(GL_FRAMEBUFFER, csmShadow->mRenderTarget->mFBO); // 绑定shadow map framebuffer
        glViewport(0, 0, csmShadow->mRenderTarget->mWidth, csmShadow->mRenderTarget->mHeight); // 设置viewport

        for (int i = 0; i < csmShadow->mLayerCount; i++) {
                glFramebufferTextureLayer(GL_FRAMEBUFFER,
                                          GL_DEPTH_ATTACHMENT,
                                          csmShadow->mRenderTarget->mDepthAttachment->getTextureID(),
                                          0, i); // 绑定第i层shadow map
                glClear(GL_DEPTH_BUFFER_BIT); // 清空深度缓冲
                this->mShadowShader->begin();
                this->mShadowShader->setMatrix<glm::mat4>("lightMatrix", lightMatrices[i]);
                for (auto &mesh: meshes) {
                        auto geometry = mesh->getGeometry();
                        glBindVertexArray(geometry->getVao());
                        this->mShadowShader->setMatrix<decltype(mesh->getModelMatrix())>("modelMatrix",
                                                                                         mesh->getModelMatrix());

                        if (mesh->getType() == ObjectType::INSTANCED_MESH) {
                                auto instanceMesh = (InstanceMesh *) mesh.get();
                                glDrawElementsInstanced(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT,
                                                        nullptr,
                                                        (int) instanceMesh->numInstanced);
                        } else {
                                glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, nullptr);
                        }
                }
                Shader::end();
        }

        /* 5.恢复状态 */
        glBindFramebuffer(GL_FRAMEBUFFER, preFbo); // 恢复原来的framebuffer
        glViewport(preViewport[0], preViewport[1], preViewport[2], preViewport[3]); // 恢复原来的viewport
}

