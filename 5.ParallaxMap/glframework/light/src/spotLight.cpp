//
// Created by Shaozheming on 2024/9/21.
//

#include "spotLight.h"

SpotLight::SpotLight()
= default;

const glm::vec3 &SpotLight::getTargetDirection() const
{
        return this->mTargetDirection;
}

void SpotLight::setTargetDirection(const glm::vec3 &targetDirection)
{
        this->mTargetDirection = targetDirection;
}

float SpotLight::getInnerAngle() const
{
        return this->mInnerAngle;
}

void SpotLight::setInnerAngle(float innerAngle)
{
        this->mInnerAngle = innerAngle;
}

float SpotLight::getOuterAngle() const
{
        return this->mOuterAngle;
}

void SpotLight::setOuterAngle(float outerAngle)
{
        if (outerAngle < this->mInnerAngle) {
                this->mOuterAngle = this->mInnerAngle;
        } else {
                this->mOuterAngle = outerAngle;
        }
}

