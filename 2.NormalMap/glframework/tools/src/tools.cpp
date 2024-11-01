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
