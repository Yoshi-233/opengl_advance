#pragma once

#include "shadow.h"

class DirectionalLightCSMShadow : public Shadow {
public:
        DirectionalLightCSMShadow();

        ~DirectionalLightCSMShadow();

        void setRenderTargetSize(int width, int height) override;

        void generateCascadeLayers(std::vector<float> &layers, float near, float far) const;
public:
        int mLayerCount = 5;
};