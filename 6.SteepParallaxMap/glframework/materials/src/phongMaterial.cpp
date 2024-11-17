//
// Created by nio on 24-9-19.
//

#include "../include/phongMaterial.h"

#include <utility>


PhongMaterial::PhongMaterial()
{
        this->mType = MaterialType::PhongMaterial;
}

void PhongMaterial::setDiffuse(std::shared_ptr<Texture> diffuse)
{
        this->mDiffuse = std::move(diffuse);
}

void PhongMaterial::setShiness(float shiness)
{
        this->mShiness = shiness;
}

void PhongMaterial::setSpecularMask(std::shared_ptr<Texture> specularMask)
{
        this->mSpecularMask = std::move(specularMask);
}

std::shared_ptr<Texture> PhongMaterial::getDiffuse() const
{
        return this->mDiffuse;
}

float PhongMaterial::getShiness() const
{
        return this->mShiness;
}

std::shared_ptr<Texture> PhongMaterial::getSpecularMask() const
{
        return this->mSpecularMask;
}


PhongMaterial::~PhongMaterial()
= default;
