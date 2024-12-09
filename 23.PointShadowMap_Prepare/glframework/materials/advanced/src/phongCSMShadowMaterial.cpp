//
// Created by nio on 24-9-19.
//

#include "../include/phongCSMShadowMaterial.h"

#include <utility>


PhongCSMShadowMaterial::PhongCSMShadowMaterial()
{
        this->mType = MaterialType::PhongCSMShadowMaterial;
}

void PhongCSMShadowMaterial::setDiffuse(std::shared_ptr<Texture> diffuse)
{
        this->mDiffuse = std::move(diffuse);
}

void PhongCSMShadowMaterial::setShiness(float shiness)
{
        this->mShiness = shiness;
}

void PhongCSMShadowMaterial::setSpecularMask(std::shared_ptr<Texture> specularMask)
{
        this->mSpecularMask = std::move(specularMask);
}

std::shared_ptr<Texture> PhongCSMShadowMaterial::getDiffuse() const
{
        return this->mDiffuse;
}

float PhongCSMShadowMaterial::getShiness() const
{
        return this->mShiness;
}

std::shared_ptr<Texture> PhongCSMShadowMaterial::getSpecularMask() const
{
        return this->mSpecularMask;
}


PhongCSMShadowMaterial::~PhongCSMShadowMaterial()
= default;

void PhongCSMShadowMaterial::setNormalMap(std::shared_ptr<Texture> normalMap)
{
        this->mNormalMap = std::move(normalMap);
}

std::shared_ptr<Texture> PhongCSMShadowMaterial::getNormalMap() const
{
        return this->mNormalMap;
}

void PhongCSMShadowMaterial::setParallaxMap(std::shared_ptr<Texture> parallaxMap)
{
        this->mParallaxMap = std::move(parallaxMap);
}

std::shared_ptr<Texture> PhongCSMShadowMaterial::getParallaxMap() const
{
        return this->mParallaxMap;
}

void PhongCSMShadowMaterial::setHeightScale(float heightScale)
{
        this->mHeightScale = heightScale;
}

float PhongCSMShadowMaterial::getHeightScale() const
{
        return this->mHeightScale;
}

void PhongCSMShadowMaterial::setLayerNum(float layerNum)
{
        this->mLayerNum = layerNum;
}

float PhongCSMShadowMaterial::getLayerNum() const
{
        return this->mLayerNum;
}
