//
// Created by Shaozheming on 2024/9/19.
//

#ifndef RENDERER_AMBIENTLIGHT_H
#define RENDERER_AMBIENTLIGHT_H

#include "light.h"

class AmbientLight : public Light {
public:
        AmbientLight() = default;

        ~AmbientLight() override = default;


};

#endif //RENDERER_AMBIENTLIGHT_H
