//
// Created by Shaozheming on 2024/9/22.
//

#include "tools.h"

void Tools::decompose(const glm::mat4 &matrix, glm::vec3 &position, glm::vec3 &eulerAngle, glm::vec3 &scale)
{
        // 四元数，用来表示旋转变换
        glm::quat quaternion;

        glm::vec3 skew;
        glm::vec4 perspective;

        // 解算旋转矩阵
        glm::decompose(matrix, scale, quaternion, position, skew, perspective);

        // 四元数转欧拉角
        auto rotation = glm::toMat4(quaternion);
        glm::extractEulerAngleXYZ(rotation, eulerAngle.x, eulerAngle.y, eulerAngle.z);

        // 转角度
        eulerAngle = glm::degrees(eulerAngle);
}

std::vector<glm::vec4> Tools::getFrustumCornersWorldSpace(const glm::mat4 &projView)
{
        const auto inv = glm::inverse(projView);

        std::vector<glm::vec4> corners{};

        for (int x = 0; x < 2; x++) {
                for (int y = 0; y < 2; y++) {
                        for (int z = 0; z < 2; z++) {
                                glm::vec4 ndc = glm::vec4(x * 2 - 1, y * 2 - 1, z * 2 - 1, 1);
                                glm::vec4 p_middle = inv * ndc;
                                auto p_world = p_middle / p_middle.w;
                                corners.push_back(p_world);
                        }
                }
        }

        return corners;
}
