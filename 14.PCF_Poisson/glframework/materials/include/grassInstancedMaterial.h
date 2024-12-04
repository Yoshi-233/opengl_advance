//
// Created by nio on 24-9-19.
//

#pragma once

#include "material.h"
#include "../../include/texture.h"

class GrassInstancedMaterial : public Material {
public:
        GrassInstancedMaterial();

        ~GrassInstancedMaterial();

        void setDiffuse(std::shared_ptr<Texture> diffuse);

        void setSpecularMask(std::shared_ptr<Texture> specularMask);

        void setOpacityMask(std::shared_ptr<Texture> opacityMask);

        void setShiness(float shiness);

        void setUVScale(float uvScale);

        [[nodiscard]] std::shared_ptr<Texture> getDiffuse() const;

        [[nodiscard]] std::shared_ptr<Texture> getSpecularMask() const;

        [[nodiscard]] std::shared_ptr<Texture> getOpacityMask() const;

        [[nodiscard]] float getShiness() const;

        [[nodiscard]] float getUVScale() const;
public:
        float mUVScale{1.0f};
        float mBrightness{1.0f};

        /* 风力 */
        float mWindStrength{1.0f};
        glm::vec3 mWindDirection{1.0f}; // 风向角度，单位为弧度
        float mPhaseScale{1.0f};

        /* 云朵 */
        std::shared_ptr<Texture> mCloudMask{nullptr};
        glm::vec3 mCloudWhiteColor{1.0f, 0.0f, 0.0f};
        glm::vec3 mCloudBlackColor{0.0f, 0.0f, 1.0f};
        float mCloudUVScale{1.0f};
        float mCloudSpeed{0.1f};
        float mCloudLerp{0.5f}; // 云朵融合系数
protected:
        std::shared_ptr<Texture> mDiffuse{nullptr};
        std::shared_ptr<Texture> mSpecularMask{nullptr};
        std::shared_ptr<Texture> mOpacityMask{nullptr};
        float mShiness{1.0f}; // 高光反射用
};
