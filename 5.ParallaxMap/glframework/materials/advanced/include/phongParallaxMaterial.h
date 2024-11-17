//
// Created by nio on 24-9-19.
//

#pragma once

#include "../../include/material.h"
#include "../../../include/texture.h"

class PhongParallaxMaterial : public Material {
public:
        PhongParallaxMaterial();
        ~PhongParallaxMaterial();
        void setDiffuse(std::shared_ptr<Texture> diffuse);
        void setSpecularMask(std::shared_ptr<Texture> specularMask);
        void setShiness(float shiness);
        void setNormalMap(std::shared_ptr<Texture> normalMap);
        void setParallaxMap(std::shared_ptr<Texture> parallaxMap);
        void setHeightScale(float heightScale);

        [[nodiscard]] std::shared_ptr<Texture> getDiffuse() const;
        [[nodiscard]] std::shared_ptr<Texture> getSpecularMask() const;
        [[nodiscard]] float getShiness() const;
        [[nodiscard]] std::shared_ptr<Texture> getNormalMap() const;
        [[nodiscard]] std::shared_ptr<Texture> getParallaxMap() const;
        [[nodiscard]] float getHeightScale() const;

protected:
        std::shared_ptr<Texture> mDiffuse{nullptr};
        std::shared_ptr<Texture> mSpecularMask{nullptr};
        std::shared_ptr<Texture> mNormalMap{nullptr};
        std::shared_ptr<Texture> mParallaxMap{nullptr};

        float mHeightScale{0.1f}; // 视差贴图高度缩放
        float mShiness{1.0f}; // 高光反射用
};
