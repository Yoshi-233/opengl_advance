//
// Created by nio on 24-9-19.
//

#ifndef RENDERER_OBJECT_H
#define RENDERER_OBJECT_H

#include "core.h"

enum class ObjectType {
        OBJECT,
        MESH,
        INSTANCED_MESH,
        SCENE,
};

class Object : public std::enable_shared_from_this<Object> {
public:
        Object();
        virtual ~Object();

public:
        void setPosition(glm::vec3 pos);
        glm::vec3 getPosition();

public:
        // 增量旋转
        void rotateX(float angle);
        void rotateY(float angle);
        void rotateZ(float angle);

        // 绝对旋转
        void setAngleX(float angle);
        void setAngleY(float angle);
        void setAngleZ(float angle);

        void setScale(glm::vec3 scale);

        glm::mat4 getModelMatrix() const;

public:
        /* 父子关系 */
        void addChild(const std::shared_ptr<Object>& child);
        std::vector<std::shared_ptr<Object>> getChildren();
        std::shared_ptr<Object> getParent();
public:
        ObjectType getType() const;

protected:
        glm::vec3 mPosition{0.0f};

        // 个体坐标系 pitch yaw roll
        float mAngleX{0.0f};
        float mAngleY{0.0f};
        float mAngleZ{0.0f};

        glm::vec3 mScale{1.0f};
protected:
        /* 父子关系 */
        std::vector<std::shared_ptr<Object>> mChildren{};
        std::shared_ptr<Object> mParent{nullptr};

protected:
        ObjectType mType{ObjectType::OBJECT};
};

#endif //RENDERER_OBJECT_H
