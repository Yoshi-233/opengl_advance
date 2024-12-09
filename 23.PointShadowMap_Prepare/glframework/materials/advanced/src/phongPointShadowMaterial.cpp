//
// Created by nio on 24-9-19.
//

#include "../include/phongPointShadowMaterial.h"

#include <utility>


PhongPointShadowMaterial::PhongPointShadowMaterial()
{
        this->mType = MaterialType::PhongPointShadowMaterial;
}

void PhongPointShadowMaterial::setDiffuse(std::shared_ptr<Texture> diffuse)
{
        this->mDiffuse = std::move(diffuse);
}

void PhongPointShadowMaterial::setShiness(float shiness)
{
        this->mShiness = shiness;
}

void PhongPointShadowMaterial::setSpecularMask(std::shared_ptr<Texture> specularMask)
{
        this->mSpecularMask = std::move(specularMask);
}

std::shared_ptr<Texture> PhongPointShadowMaterial::getDiffuse() const
{
        return this->mDiffuse;
}

float PhongPointShadowMaterial::getShiness() const
{
        return this->mShiness;
}

std::shared_ptr<Texture> PhongPointShadowMaterial::getSpecularMask() const
{
        return this->mSpecularMask;
}


PhongPointShadowMaterial::~PhongPointShadowMaterial()
= default;

void PhongPointShadowMaterial::setNormalMap(std::shared_ptr<Texture> normalMap)
{
        this->mNormalMap = std::move(normalMap);
}

std::shared_ptr<Texture> PhongPointShadowMaterial::getNormalMap() const
{
        return this->mNormalMap;
}

void PhongPointShadowMaterial::setParallaxMap(std::shared_ptr<Texture> parallaxMap)
{
        this->mParallaxMap = std::move(parallaxMap);
}

std::shared_ptr<Texture> PhongPointShadowMaterial::getParallaxMap() const
{
        return this->mParallaxMap;
}

void PhongPointShadowMaterial::setHeightScale(float heightScale)
{
        this->mHeightScale = heightScale;
}

float PhongPointShadowMaterial::getHeightScale() const
{
        return this->mHeightScale;
}

void PhongPointShadowMaterial::setLayerNum(float layerNum)
{
        this->mLayerNum = layerNum;
}

float PhongPointShadowMaterial::getLayerNum() const
{
        return this->mLayerNum;
}
