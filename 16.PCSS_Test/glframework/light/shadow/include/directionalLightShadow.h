#pragma once

#include "shadow.h"

class DirectionalLightShadow : public Shadow {
public:
        DirectionalLightShadow();

        ~DirectionalLightShadow();

        void setRenderTargetSize(int width, int height) override;

        [[nodiscard]] glm::mat4 getLightMatrix(glm::mat4 lightModelMatrix) const;
};