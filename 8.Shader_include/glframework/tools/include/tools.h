//
// Created by Shaozheming on 2024/9/22.
//

#ifndef READINGMODELS_TOOLS_H
#define READINGMODELS_TOOLS_H

#include "core.h"

class Tools {
public:
        // 传入一个矩阵，返回其中的旋转信息，缩放信息，位置
        static void decompose(const glm::mat4 &matrix, glm::vec3 &position, glm::vec3 &eulerAngle,
                              glm::vec3 &scale);
};

#endif //READINGMODELS_TOOLS_H
