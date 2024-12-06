//
// Created by nio on 24-11-1.
//
#include "renderer.h"
#include "scene.h"
#include "phongMaterial.h"
#include "phongEnvMaterial.h"
#include "phongInstancedMaterial.h"
#include <memory>
#include "../../../application/app/Application.h"

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
