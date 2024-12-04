#pragma once

#include "../../../include/core.h"
#include "../../../framebuffer/include/framebuffer.h"
#include "../../../../application/camera/include/camera.h"

class Shadow {
public:
        Shadow();
        ~Shadow();

        virtual void setRenderTargetSize(int width, int height) = 0;
public:
        std::shared_ptr<Camera> mCamera{nullptr};
        std::shared_ptr<Framebuffer> mRenderTarget{nullptr};

        float mBias{0.0003f};
        float mPcfRadius{0.01f}; // pcf radius, pcf采样半径
        float mDiskTightness{1.0f}; // shadow disk thickness, 阴影圆盘厚度
};