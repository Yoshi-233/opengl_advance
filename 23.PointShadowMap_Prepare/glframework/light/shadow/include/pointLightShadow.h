#pragma once

#include "shadow.h"

class PointLightShadow : public Shadow {
public:
        PointLightShadow();

        ~PointLightShadow();

        void setRenderTargetSize(int width, int height) override;

        [[nodiscard]] glm::mat4 getLightMatrix(glm::mat4 lightModelMatrix) const;
};