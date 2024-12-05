//
// Created by nio on 24-9-19.
//

#include "../include/phongInstancedMaterial.h"

#include <utility>


PhongInstancedMaterial::PhongInstancedMaterial()
{
        this->mType = MaterialType::PhongInstancedMaterial;
}

void PhongInstancedMaterial::setDiffuse(std::shared_ptr<Texture> diffuse)
{
        this->mDiffuse = std::move(diffuse);
}

void PhongInstancedMaterial::setShiness(float shiness)
{
        this->mShiness = shiness;
}

void PhongInstancedMaterial::setSpecularMask(std::shared_ptr<Texture> specularMask)
{
        this->mSpecularMask = std::move(specularMask);
}

std::shared_ptr<Texture> PhongInstancedMaterial::getDiffuse() const
{
        return this->mDiffuse;
}

float PhongInstancedMaterial::getShiness() const
{
        return this->mShiness;
}

std::shared_ptr<Texture> PhongInstancedMaterial::getSpecularMask() const
{
        return this->mSpecularMask;
}


PhongInstancedMaterial::~PhongInstancedMaterial()
= default;
