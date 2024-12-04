//
// Created by nio on 24-9-19.
//

#pragma once

#include "../../include/material.h"
#include "../../../include/texture.h"

class PhongShadowMaterial : public Material {
public:
        PhongShadowMaterial();
        ~PhongShadowMaterial();
        void setDiffuse(std::shared_ptr<Texture> diffuse);
        void setSpecularMask(std::shared_ptr<Texture> specularMask);
        void setShiness(float shiness);
        void setNormalMap(std::shared_ptr<Texture> normalMap);
        void setParallaxMap(std::shared_ptr<Texture> parallaxMap);
        void setHeightScale(float heightScale);
        void setLayerNum(float layerNum);

        [[nodiscard]] std::shared_ptr<Texture> getDiffuse() const;
        [[nodiscard]] std::shared_ptr<Texture> getSpecularMask() const;
        [[nodiscard]] float getShiness() const;
        [[nodiscard]] std::shared_ptr<Texture> getNormalMap() const;
        [[nodiscard]] std::shared_ptr<Texture> getParallaxMap() const;
        [[nodiscard]] float getHeightScale() const;
        [[nodiscard]] float getLayerNum() const;

protected:
        std::shared_ptr<Texture> mDiffuse{nullptr};
        std::shared_ptr<Texture> mSpecularMask{nullptr};
        std::shared_ptr<Texture> mNormalMap{nullptr};
        std::shared_ptr<Texture> mParallaxMap{nullptr};

        float mHeightScale{0.1f}; // 视差贴图高度缩放
        float mLayerNum{10.0f}; // 视差贴图层数
        float mShiness{1.0f}; // 高光反射用
public:
        float mBias{0.0f}; // shadow bias, 阴影偏移量
};
