//
// Created by nio on 24-11-1.
//

#include "renderer.h"
#include "scene.h"
#include "phongEnvMaterial.h"
#include <memory>
#include "../../../application/app/Application.h"


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
