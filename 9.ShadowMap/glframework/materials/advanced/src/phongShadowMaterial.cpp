//
// Created by nio on 24-9-19.
//

#include "../include/phongShadowMaterial.h"

#include <utility>


PhongShadowMaterial::PhongShadowMaterial()
{
        this->mType = MaterialType::PhongShadowMaterial;
}

void PhongShadowMaterial::setDiffuse(std::shared_ptr<Texture> diffuse)
{
        this->mDiffuse = std::move(diffuse);
}

void PhongShadowMaterial::setShiness(float shiness)
{
        this->mShiness = shiness;
}

void PhongShadowMaterial::setSpecularMask(std::shared_ptr<Texture> specularMask)
{
        this->mSpecularMask = std::move(specularMask);
}

std::shared_ptr<Texture> PhongShadowMaterial::getDiffuse() const
{
        return this->mDiffuse;
}

float PhongShadowMaterial::getShiness() const
{
        return this->mShiness;
}

std::shared_ptr<Texture> PhongShadowMaterial::getSpecularMask() const
{
        return this->mSpecularMask;
}


PhongShadowMaterial::~PhongShadowMaterial()
= default;

void PhongShadowMaterial::setNormalMap(std::shared_ptr<Texture> normalMap)
{
        this->mNormalMap = std::move(normalMap);
}

std::shared_ptr<Texture> PhongShadowMaterial::getNormalMap() const
{
        return this->mNormalMap;
}

void PhongShadowMaterial::setParallaxMap(std::shared_ptr<Texture> parallaxMap)
{
        this->mParallaxMap = std::move(parallaxMap);
}

std::shared_ptr<Texture> PhongShadowMaterial::getParallaxMap() const
{
        return this->mParallaxMap;
}

void PhongShadowMaterial::setHeightScale(float heightScale)
{
        this->mHeightScale = heightScale;
}

float PhongShadowMaterial::getHeightScale() const
{
        return this->mHeightScale;
}

void PhongShadowMaterial::setLayerNum(float layerNum)
{
        this->mLayerNum = layerNum;
}

float PhongShadowMaterial::getLayerNum() const
{
        return this->mLayerNum;
}
