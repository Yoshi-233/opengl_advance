//
// Created by nio on 24-9-19.
//

#include "../include/phongEnvMaterial.h"

#include <utility>


PhongEnvMaterial::PhongEnvMaterial()
{
        this->mType = MaterialType::PhongEnvMaterial;
}

void PhongEnvMaterial::setDiffuse(std::shared_ptr<Texture> diffuse)
{
        this->mDiffuse = std::move(diffuse);
}

void PhongEnvMaterial::setShiness(float shiness)
{
        this->mShiness = shiness;
}

void PhongEnvMaterial::setSpecularMask(std::shared_ptr<Texture> specularMask)
{
        this->mSpecularMask = std::move(specularMask);
}

std::shared_ptr<Texture> PhongEnvMaterial::getDiffuse() const
{
        return this->mDiffuse;
}

float PhongEnvMaterial::getShiness() const
{
        return this->mShiness;
}

std::shared_ptr<Texture> PhongEnvMaterial::getSpecularMask() const
{
        return this->mSpecularMask;
}

void PhongEnvMaterial::setEnv(std::shared_ptr<Texture> env)
{
        this->mEnv = std::move(env);
}

std::shared_ptr<Texture> PhongEnvMaterial::getEnv() const
{
        return this->mEnv;
}

PhongEnvMaterial::~PhongEnvMaterial()
= default;
