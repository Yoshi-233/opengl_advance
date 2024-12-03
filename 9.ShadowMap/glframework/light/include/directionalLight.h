//
// Created by Shaozheming on 2024/9/19.
//

#ifndef RENDERER_DIRECTIONALLIGHT_H
#define RENDERER_DIRECTIONALLIGHT_H

#include "light.h"

class DirectionalLight : public Light {
public:
        DirectionalLight() = default;

        ~DirectionalLight() override = default;

};

#endif //RENDERER_DIRECTIONALLIGHT_H
