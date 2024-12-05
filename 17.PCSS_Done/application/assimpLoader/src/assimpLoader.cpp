//
// Created by Shaozheming on 2024/9/22.
//

#include "assimpLoader.h"
#include <../../glframework/tools/include/tools.h>
#include <../../glframework/materials/include/phongMaterial.h>

std::shared_ptr<Object> AssimpLoader::loadModel(const std::string &path)
{
        // 路径所在目录
        std::size_t lastIndex = path.find_last_of("//");
        std::string rootPath = path.substr(0, lastIndex + 1);

        std::shared_ptr<Object> rootNode = std::make_shared<Object>();
        Assimp::Importer importer;

        // 四边面三角形化，如果没有法线，则生成法线
        auto scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);

        // 验证是否读取成功
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
                return nullptr;
        }

        AssimpLoader::processNode(scene->mRootNode, rootNode, scene, rootPath);

        return rootNode;
}

void AssimpLoader::processNode(aiNode *aiNode, const std::shared_ptr<Object> &parent, const aiScene *scene,
                               std::string &rootPath)
{
        std::shared_ptr<Object> node = std::make_shared<Object>();
        parent->addChild(node);
        auto localMatrix = AssimpLoader::getMatrix4f(aiNode->mTransformation);

        // 位置旋转缩放
        glm::vec3 position, scale, eulerAngles;
        Tools::decompose(localMatrix, position, eulerAngles, scale);
        node->setPosition(position);
        node->setAngleX(eulerAngles.x);
        node->setAngleY(eulerAngles.y);
        node->setAngleZ(eulerAngles.z);
        node->setScale(scale);

        // 检查有没有mesh并解析
        for (unsigned int i = 0; i < aiNode->mNumMeshes; i++) {
                aiMesh *aiMesh = scene->mMeshes[aiNode->mMeshes[i]];
                auto mesh = AssimpLoader::processMesh(aiMesh, scene, rootPath);
                node->addChild(mesh);
        }

        // 子节点
        for (unsigned int i = 0; i < aiNode->mNumChildren; i++) {
                processNode(aiNode->mChildren[i], node, scene, rootPath);
        }


}

glm::mat4 AssimpLoader::getMatrix4f(aiMatrix4x4 &matrix)
{
        glm::mat4 to{
                matrix.a1, matrix.a2, matrix.a3, matrix.a4,
                matrix.b1, matrix.b2, matrix.b3, matrix.b4,
                matrix.c1, matrix.c2, matrix.c3, matrix.c4,
                matrix.d1, matrix.d2, matrix.d3, matrix.d4
        };

        return to;
}

std::shared_ptr<Mesh> AssimpLoader::processMesh(aiMesh *aiMesh, const aiScene *scene, std::string &rootPath)
{
        std::vector<float> positions, normals, uvs;
        std::vector<unsigned int> indices;

        for (unsigned int i = 0; i < aiMesh->mNumVertices; i++) {
                // 位置
                positions.push_back(aiMesh->mVertices[i].x);
                positions.push_back(aiMesh->mVertices[i].y);
                positions.push_back(aiMesh->mVertices[i].z);

                // 法线
                if (aiMesh->HasNormals()) {
                        normals.push_back(aiMesh->mNormals[i].x);
                        normals.push_back(aiMesh->mNormals[i].y);
                        normals.push_back(aiMesh->mNormals[i].z);
                }

                // uv, 同一个顶点可能有多个uv
                // 一般第0号uv通道为贴图的uv
                if (aiMesh->HasTextureCoords(0)) {
                        uvs.push_back(aiMesh->mTextureCoords[0][i].x);
                        uvs.push_back(aiMesh->mTextureCoords[0][i].y);
                } else {
                        uvs.push_back(0.0f);
                        uvs.push_back(0.0f);
                }
        }

        // 解析索引
        for (unsigned int i = 0; i < aiMesh->mNumFaces; i++) {
                aiFace face = aiMesh->mFaces[i];
                for (unsigned int j = 0; j < face.mNumIndices; j++) {
                        indices.push_back(face.mIndices[j]);
                }
        }

        auto geometry = std::make_shared<Geometry>(positions, normals, uvs, indices);
        auto material = std::make_shared<PhongMaterial>();

        // INFO(std::string(PROJECT_DIR_INAPP));

        if (aiMesh->mMaterialIndex >= 0) {
                std::shared_ptr<Texture> texture;
                aiMaterial *aiMaterial = scene->mMaterials[aiMesh->mMaterialIndex];

                // 读取diffuse
                texture = AssimpLoader::processTexture(aiMaterial, aiTextureType_DIFFUSE, scene, rootPath);
                if (!texture) {
                        texture = std::make_shared<Texture>(std::string(PROJECT_DIR_INAPP) + "/assets/textures/box.png",
                                                            0);
                }

                // 读取specular
                auto specularTexture = AssimpLoader::processTexture(aiMaterial,
                                                                    aiTextureType_SPECULAR, scene, rootPath);

                texture->setUnit(0);
                material->setDiffuse(texture);
                if (!specularTexture) {
                        specularTexture = std::make_shared<Texture>(std::string(PROJECT_DIR_INAPP) + "/assets/textures/box.png",
                                                            1);
                }
                specularTexture->setUnit(1);
                material->setSpecularMask(specularTexture);
        } else {
                material->setDiffuse(
                        std::make_shared<Texture>(std::string(PROJECT_DIR_INAPP) + "/assets/textures/box.png",
                                                  0));
        }

        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(geometry, material);
        return mesh;
}

std::shared_ptr<Texture>
AssimpLoader::processTexture(const aiMaterial *aiMaterial, const aiTextureType &type, const aiScene *scene,
                             const std::string &rootPath)
{
        std::shared_ptr<Texture> texture;

        // 解析材质属性
        // 获取图片读取路径
        aiString aiPath;
        // 拿到漫反射0号贴图路径, 正常第0张为漫反射贴图
        aiMaterial->Get(AI_MATKEY_TEXTURE(type, 0), aiPath);
        if (aiPath.length == 0) {
                return nullptr;
        }

        // 判断是否是嵌入式纹理
        auto aitexture = scene->GetEmbeddedTexture(aiPath.C_Str());
        if (aitexture) {
                // 读取嵌入式纹理
                auto *data = reinterpret_cast<unsigned char *>(aitexture->pcData);
                auto width = aitexture->mWidth; // png和jpg, width代表整张图片大小
                auto height = aitexture->mHeight;
                texture = Texture::createTextureFromMemory(aiPath.C_Str(), 0,
                                                           data, (int) width, (int) height);


        } else {
                // 读取图片
                auto picPath = std::string(aiPath.C_Str());
                size_t idx = std::string(aiPath.C_Str()).find_last_of('\\');
                if (idx != std::string::npos) {
                        picPath = std::string(aiPath.C_Str()).replace(idx, 1, "/");
                }
                std::string fullPath = std::string(PROJECT_DIR_INAPP) + "/" +
                                       rootPath + picPath;
                INFO(fullPath);
                texture = Texture::createTexture(fullPath, 0);
        }

        return texture;
}
