//
// Created by nio on 24-9-19.
//

#include "../include/phongNormalMaterial.h"

#include <utility>


PhongNormalMaterial::PhongNormalMaterial()
{
        this->mType = MaterialType::PhongNormalMaterial;
}

void PhongNormalMaterial::setDiffuse(std::shared_ptr<Texture> diffuse)
{
        this->mDiffuse = std::move(diffuse);
}

void PhongNormalMaterial::setShiness(float shiness)
{
        this->mShiness = shiness;
}

void PhongNormalMaterial::setSpecularMask(std::shared_ptr<Texture> specularMask)
{
        this->mSpecularMask = std::move(specularMask);
}

std::shared_ptr<Texture> PhongNormalMaterial::getDiffuse() const
{
        return this->mDiffuse;
}

float PhongNormalMaterial::getShiness() const
{
        return this->mShiness;
}

std::shared_ptr<Texture> PhongNormalMaterial::getSpecularMask() const
{
        return this->mSpecularMask;
}


PhongNormalMaterial::~PhongNormalMaterial()
= default;

void PhongNormalMaterial::setNormalMap(std::shared_ptr<Texture> normalMap)
{
        this->mNormalMap = normalMap;
}

std::shared_ptr<Texture> PhongNormalMaterial::getNormalMap() const
{
        return this->mNormalMap;
}
