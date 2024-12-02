//
// Created by nio on 24-9-19.
//

#pragma once

#include "../../include/object.h"
#include "../../include/geometry.h"
#include "../../materials/include/material.h"

class Mesh : public Object {

public:
        Mesh(const std::shared_ptr<Geometry>& geometry, const std::shared_ptr<Material>& material);

        ~Mesh();

        [[nodiscard]] std::shared_ptr<Geometry> getGeometry() const;

        [[nodiscard]] std::shared_ptr<Material> getMaterial() const;

public:
        std::shared_ptr<Geometry> mGeometry{nullptr};
        std::shared_ptr<Material> mMaterial{nullptr};

};

