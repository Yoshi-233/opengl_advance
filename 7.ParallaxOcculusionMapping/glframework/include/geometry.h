//
// Created by Shaozheming on 2024/9/16.
//

#ifndef GEOMETRY_GEOMETRY_H
#define GEOMETRY_GEOMETRY_H

#include "core.h"
#include <memory>

class Geometry {
public:
        Geometry();

        Geometry(const std::vector<float> &positions, const std::vector<float> &normals,
                 const std::vector<float> &uvs, const std::vector<unsigned int> &indices);

        Geometry(const std::vector<float> &positions, const std::vector<float> &normals,
                 const std::vector<float> &uvs, const std::vector<float> &colors,
                 const std::vector<unsigned int> &indices);

        Geometry(const std::vector<float> &positions, const std::vector<float> &normals,
                 const std::vector<float> &uvs,
                 const std::vector<unsigned int> &indices, const std::vector<float> &tangents);

        ~Geometry();

        static std::shared_ptr<Geometry> createBox(float size);

        static std::shared_ptr<Geometry> createSphere(float radius);

        static std::shared_ptr<Geometry> createPlane(float width, float height);

        static std::shared_ptr<Geometry> createScreenPlane();

        [[nodiscard]] GLuint getVao() const
        { return mVao; };

        [[nodiscard]] GLuint getPosVbo() const
        { return mPosVbo; };

        [[nodiscard]] GLuint getUvVbo() const
        { return mUvVbo; };

        [[nodiscard]] GLuint getNormalVbo() const
        { return mNormalVbo; };

        [[nodiscard]] GLuint getEbo() const
        { return mEbo; };

        [[nodiscard]] GLuint getTangentVbo() const
        { return mTangentVbo; };

        [[nodiscard]] int getIndicesCount() const
        { return mIndicesCount; };

public:
        GLuint mVao{0};
        GLuint mPosVbo{0};
        GLuint mUvVbo{0};
        GLuint mColorVbo{0};
        GLuint mNormalVbo{0};
        GLuint mEbo{0};

        GLuint mTangentVbo{0};

        int mIndicesCount{0};

};

#endif //GEOMETRY_GEOMETRY_H
