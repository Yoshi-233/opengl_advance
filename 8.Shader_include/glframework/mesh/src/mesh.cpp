//
// Created by nio on 24-9-19.
//
#include "../include/mesh.h"

Mesh::Mesh(const std::shared_ptr<Geometry>& geometry, const std::shared_ptr<Material>& material)
{
        this->mGeometry = geometry;
        this->mMaterial = material;
        this->mType = ObjectType::MESH;
}

Mesh::~Mesh()
= default;

std::shared_ptr<Geometry> Mesh::getGeometry() const
{
        return this->mGeometry;
}

std::shared_ptr<Material> Mesh::getMaterial() const
{
        return this->mMaterial;
}
