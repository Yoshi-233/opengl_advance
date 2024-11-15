//
// Created by nio on 24-9-19.
//

#pragma once

#include "material.h"
#include "../../include/texture.h"

class PhongEnvMaterial : public Material {
public:
        PhongEnvMaterial();
        ~PhongEnvMaterial();
        void setDiffuse(std::shared_ptr<Texture> diffuse);
        void setEnv(std::shared_ptr<Texture> env);
        void setSpecularMask(std::shared_ptr<Texture> specularMask);
        void setShiness(float shiness);

        [[nodiscard]] std::shared_ptr<Texture> getDiffuse() const;
        [[nodiscard]] std::shared_ptr<Texture> getEnv() const;
        [[nodiscard]] std::shared_ptr<Texture> getSpecularMask() const;
        [[nodiscard]] float getShiness() const;

protected:
        std::shared_ptr<Texture> mDiffuse{nullptr};
        std::shared_ptr<Texture> mEnv{nullptr};
        std::shared_ptr<Texture> mSpecularMask{nullptr};
        float mShiness{1.0f}; // 高光反射用
};
