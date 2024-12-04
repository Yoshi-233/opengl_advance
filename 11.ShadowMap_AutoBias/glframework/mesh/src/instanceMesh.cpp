//
// Created by nio on 24-9-19.
//
#include "../include/instanceMesh.h"
#include <algorithm>

InstanceMesh::InstanceMesh(const std::shared_ptr<Geometry> &geometry, const std::shared_ptr<Material> &material,
                           const unsigned int numInstanced) : Mesh(geometry, material)
{
        this->mType = ObjectType::INSTANCED_MESH;
        this->numInstanced = numInstanced;

        this->instanceData.resize(numInstanced);

        glGenBuffers(1, &this->mMatrixVBO);
        glBindBuffer(GL_ARRAY_BUFFER, this->mMatrixVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * numInstanced,
                     this->instanceData.data(), GL_DYNAMIC_DRAW);

        glBindVertexArray(this->mGeometry->getVao());
        glBindBuffer(GL_ARRAY_BUFFER, this->mMatrixVBO);

        for (int i = 0; i < 4; i++) {
                glEnableVertexAttribArray(4 + i);
                glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE,
                                      sizeof(glm::mat4), (void *) (sizeof(float) * 4 * i));

                glVertexAttribDivisor(4 + i, 1); // 实例渲染
        }
        glBindVertexArray(0);
}

InstanceMesh::~InstanceMesh()
= default;

void InstanceMesh::updataMatrices()
{
        glBindBuffer(GL_ARRAY_BUFFER, this->mMatrixVBO);

        // 如果使用glBufferData，会重新分配显存, 第二个参数是offset
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * this->numInstanced, this->instanceData.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InstanceMesh::sortMatrices(const glm::mat4 &viewMatrix)
{
        std::sort(
                this->instanceData.begin(),
                this->instanceData.end(),
                [viewMatrix](const glm::mat4 &m1, const glm::mat4 &m2) {
                        // 1.计算相机坐标系，此时这里的坐标系是相对父亲节点的局部坐标系
                        auto modelMatrixM1 = m1;
                        auto worldPosM1 = modelMatrixM1 * glm::vec4(0, 0, 0, 1);
                        auto cameraPosM1 = viewMatrix * worldPosM1; // 相机坐标系

                        // auto posM2 = m2->getPosition();
                        auto modelMatrixM2 = m2;
                        auto worldPosM2 = modelMatrixM2 * glm::vec4(0, 0, 0, 1);
                        auto cameraPosM2 = viewMatrix * worldPosM2; // 相机坐标系

                        // 2.根据距离排序, 升序
                        return cameraPosM1.z < cameraPosM2.z;
                });
}


