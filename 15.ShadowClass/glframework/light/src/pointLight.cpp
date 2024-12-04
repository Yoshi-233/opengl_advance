//
// Created by nio on 24-9-20.
//

#include "pointLight.h"
#include <tuple>

float PointLight::getKc() const
{
        return this->mKc;
}

float PointLight::getK1() const
{
        return this->mK1;
}

float PointLight::getK2() const
{
        return this->mK2;
}

void PointLight::setK2(float k2)
{
        this->mK2 = k2;
}

void PointLight::setK1(float k1)
{
        this->mK1 = k1;
}

void PointLight::setKc(float kc)
{
        this->mKc = kc;
}

void PointLight::setParameters(float dist)
{
        this->mKc = 1.0f;
        if (dist == 7.0f) {
                this->mK1 = 0.7f;
                this->mKc = 1.8f;
        } else if (dist == 13.0f) {
                this->mK1 = 0.35f;
                this->mKc = 0.44f;
        } else if (dist == 20.0f) {
                this->mK1 = 0.22f;
                this->mKc = 0.20f;
        } else if (dist == 32.0f) {
                this->mK1 = 0.14f;
                this->mKc = 0.07f;
        } else if (dist == 50.0f) {
                this->mK1 = 0.09f;
                this->mKc = 0.032f;
        } else if (dist == 65.0f) {
                this->mK1 = 0.07f;
                this->mKc = 0.017f;
        } else if (dist == 100.0f) {
                this->mK1 = 0.045f;
                this->mKc = 0.0075f;
        } else if (dist == 160.0f) {
                this->mK1 = 0.027f;
                this->mKc = 0.0028f;
        } else if (dist == 200.0f) {
                this->mK1 = 0.022f;
                this->mKc = 0.0019f;
        } else if (dist == 325.0f) {
                this->mK1 = 0.014f;
                this->mKc = 0.0007f;
        } else if (dist == 600.0f) {
                this->mK1 = 0.007f;
                this->mKc = 0.0002f;
        } else if (dist == 3250.0f) {
                this->mK1 = 0.0014f;
                this->mKc = 0.000007f;
        } else {
                this->mK1 = 1.0f;
                this->mKc = 1.0f;
        }
}
