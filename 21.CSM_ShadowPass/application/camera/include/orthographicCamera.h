//
// Created by Shaozheming on 2024/9/14.
//

#pragma once

#include "camera.h"

class OrthographicCamera : public Camera {
public:
        OrthographicCamera(float left, float right, float top, float bottom, float near, float far);
        ~OrthographicCamera();

        [[nodiscard]] glm::mat4 getProjectionMatrix() const override;

        void scale(float deltaScale) override;
public:
        float mL{0.0f};
        float mR{0.0f};
        float mTop{0.0f};
        float mBottom{0.0f};
        float mScale{0.0f};
};
