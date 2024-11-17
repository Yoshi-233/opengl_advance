//
// Created by nio on 24-9-19.
//

#include "../include/phongParallaxMaterial.h"

#include <utility>


PhongParallaxMaterial::PhongParallaxMaterial()
{
        this->mType = MaterialType::PhongParallaxMaterial;
}

void PhongParallaxMaterial::setDiffuse(std::shared_ptr<Texture> diffuse)
{
        this->mDiffuse = std::move(diffuse);
}

void PhongParallaxMaterial::setShiness(float shiness)
{
        this->mShiness = shiness;
}

void PhongParallaxMaterial::setSpecularMask(std::shared_ptr<Texture> specularMask)
{
        this->mSpecularMask = std::move(specularMask);
}

std::shared_ptr<Texture> PhongParallaxMaterial::getDiffuse() const
{
        return this->mDiffuse;
}

float PhongParallaxMaterial::getShiness() const
{
        return this->mShiness;
}

std::shared_ptr<Texture> PhongParallaxMaterial::getSpecularMask() const
{
        return this->mSpecularMask;
}


PhongParallaxMaterial::~PhongParallaxMaterial()
= default;

void PhongParallaxMaterial::setNormalMap(std::shared_ptr<Texture> normalMap)
{
        this->mNormalMap = std::move(normalMap);
}

std::shared_ptr<Texture> PhongParallaxMaterial::getNormalMap() const
{
        return this->mNormalMap;
}

void PhongParallaxMaterial::setParallaxMap(std::shared_ptr<Texture> parallaxMap)
{
        this->mParallaxMap = std::move(parallaxMap);
}

std::shared_ptr<Texture> PhongParallaxMaterial::getParallaxMap() const
{
        return this->mParallaxMap;
}

void PhongParallaxMaterial::setHeightScale(float heightScale)
{
        this->mHeightScale = heightScale;
}

float PhongParallaxMaterial::getHeightScale() const
{
        return this->mHeightScale;
}
