//
// Created by nio on 24-9-20.
//

#pragma once

#include "material.h"
#include "../../include/texture.h"

class CubeMaterial : public Material {
public:
        CubeMaterial();
        ~CubeMaterial();
public:
        std::shared_ptr<Texture> mDiffuse;
};

