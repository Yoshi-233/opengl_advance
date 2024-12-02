//
// Created by Shaozheming on 2024/9/22.
//

#pragma once

#include "../../glframework/include/core.h"
#include "../../glframework/include/object.h"
#include "../../../glframework/mesh/include/mesh.h"
#include "../../glframework/include/texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class AssimpLoader {
public:
        static std::shared_ptr<Object> loadModel(const std::string& path);

private:
        static void processNode(aiNode* aiNode,  const std::shared_ptr<Object>& parent, const aiScene* scene,
                                std::string& rootPath);
        static std::shared_ptr<Mesh> processMesh(aiMesh* aiMesh, const aiScene* scene, std::string& rootPath);
        static std::shared_ptr<Texture> processTexture(const aiMaterial* aiMaterial,
                                                       const aiTextureType& type, const aiScene* scene,
                                                       const std::string& rootPath);

        static glm::mat4 getMatrix4f(aiMatrix4x4& matrix);
};

