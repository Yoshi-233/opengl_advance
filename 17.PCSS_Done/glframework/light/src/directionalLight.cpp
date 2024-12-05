//
// Created by Shaozheming on 2024/9/19.
//

#include "directionalLight.h"
#include "../shadow/include/directionalLightShadow.h"

DirectionalLight::DirectionalLight()
{
        this->mShadow = std::make_shared<DirectionalLightShadow>();
}

DirectionalLight::~DirectionalLight() = default;