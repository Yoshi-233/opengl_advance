#pragma once

#include "shadow.h"

class DirectionalLightCSMShadow : public Shadow {
public:
        DirectionalLightCSMShadow();

        ~DirectionalLightCSMShadow();

        void setRenderTargetSize(int width, int height) override;

        void generateCascadeLayers(std::vector<float> &layers, float near, float far) const;

        static glm::mat4 getLightMatrix(std::shared_ptr<Camera> &camera, glm::vec3 LightDir,
                                        float near, float far);

        static std::vector<glm::mat4> getLightMatrices(std::shared_ptr<Camera> &camera,
                                                       glm::vec3 LightDir,
                                                       std::vector<float> &clips);

public:
        int mLayerCount{5};
};