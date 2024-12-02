//
// Created by nio on 24-9-19.
//

#include "../include/grassInstancedMaterial.h"

#include <utility>


GrassInstancedMaterial::GrassInstancedMaterial()
{
        this->mType = MaterialType::GrassInstancedMaterial;
}

void GrassInstancedMaterial::setDiffuse(std::shared_ptr<Texture> diffuse)
{
        this->mDiffuse = std::move(diffuse);
}

void GrassInstancedMaterial::setShiness(float shiness)
{
        this->mShiness = shiness;
}

void GrassInstancedMaterial::setSpecularMask(std::shared_ptr<Texture> specularMask)
{
        this->mSpecularMask = std::move(specularMask);
}

std::shared_ptr<Texture> GrassInstancedMaterial::getDiffuse() const
{
        return this->mDiffuse;
}

float GrassInstancedMaterial::getShiness() const
{
        return this->mShiness;
}

std::shared_ptr<Texture> GrassInstancedMaterial::getSpecularMask() const
{
        return this->mSpecularMask;
}

void GrassInstancedMaterial::setOpacityMask(std::shared_ptr<Texture> opacityMask)
{
        this->mOpacityMask = std::move(opacityMask);
}

std::shared_ptr<Texture> GrassInstancedMaterial::getOpacityMask() const
{
        return this->mOpacityMask;
}

void GrassInstancedMaterial::setUVScale(float uvScale)
{
        this->mUVScale = uvScale;
}

float GrassInstancedMaterial::getUVScale() const
{
        return this->mUVScale;
}


GrassInstancedMaterial::~GrassInstancedMaterial()
= default;
