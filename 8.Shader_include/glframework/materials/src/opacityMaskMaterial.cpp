//
// Created by nio on 24-9-19.
//

#include "../include/opacityMaskMaterial.h"

#include <utility>


OpacityMaskMaterial::OpacityMaskMaterial()
{
        this->mType = MaterialType::OpacityMaskMaterial;
}

void OpacityMaskMaterial::setDiffuse(std::shared_ptr<Texture> diffuse)
{
        this->mDiffuse = std::move(diffuse);
}

void OpacityMaskMaterial::setShiness(float shiness)
{
        this->mShiness = shiness;
}

void OpacityMaskMaterial::setOpacityMask(std::shared_ptr<Texture> opacityMask)
{
        this->mOpacityMask = std::move(opacityMask);
}

std::shared_ptr<Texture> OpacityMaskMaterial::getDiffuse() const
{
        return this->mDiffuse;
}

float OpacityMaskMaterial::getShiness() const
{
        return this->mShiness;
}

std::shared_ptr<Texture> OpacityMaskMaterial::getOpacityMask() const
{
        return this->mOpacityMask;
}


OpacityMaskMaterial::~OpacityMaskMaterial()
= default;
