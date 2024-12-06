//
// Created by Shaozheming on 2024/9/19.
//

#include "directionalLight.h"
#include "../shadow/include/directionalLightCSMShadow.h"

DirectionalLight::DirectionalLight()
{
        this->mShadow = std::make_shared<DirectionalLightCSMShadow>();
}

DirectionalLight::~DirectionalLight() = default;