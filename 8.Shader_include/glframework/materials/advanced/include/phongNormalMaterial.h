//
// Created by nio on 24-9-19.
//

#pragma once

#include "../../include/material.h"
#include "../../../include/texture.h"

class PhongNormalMaterial : public Material {
public:
        PhongNormalMaterial();
        ~PhongNormalMaterial();
        void setDiffuse(std::shared_ptr<Texture> diffuse);
        void setSpecularMask(std::shared_ptr<Texture> specularMask);
        void setShiness(float shiness);
        void setNormalMap(std::shared_ptr<Texture> normalMap);

        [[nodiscard]] std::shared_ptr<Texture> getDiffuse() const;
        [[nodiscard]] std::shared_ptr<Texture> getSpecularMask() const;
        [[nodiscard]] float getShiness() const;
        [[nodiscard]] std::shared_ptr<Texture> getNormalMap() const;

protected:
        std::shared_ptr<Texture> mDiffuse{nullptr};
        std::shared_ptr<Texture> mSpecularMask{nullptr};
        std::shared_ptr<Texture> mNormalMap{nullptr};
        float mShiness{1.0f}; // 高光反射用
};
