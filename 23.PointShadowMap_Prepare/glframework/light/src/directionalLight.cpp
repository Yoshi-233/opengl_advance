//
// Created by Shaozheming on 2024/9/19.
//

#include "directionalLight.h"
#include "../shadow/include/directionalLightCSMShadow.h"
#include "../shadow/include/directionalLightShadow.h"

DirectionalLight::DirectionalLight(bool useCSM)
{
        if (useCSM) {
                this->mShadow = std::make_shared<DirectionalLightCSMShadow>();
                this->mUseCSM = true;
        } else {
                this->mShadow = std::make_shared<DirectionalLightShadow>();
                this->mUseCSM = false;
        }

}

DirectionalLight::~DirectionalLight() = default;