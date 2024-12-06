//
// Created by nio on 24-9-20.
//

#pragma once

#include "material.h"
#include "../../include/texture.h"

class ScreenMaterial : public Material {
public:
        ScreenMaterial();
        ~ScreenMaterial();
public:
        std::shared_ptr<Texture> mScreenTexture;
};

