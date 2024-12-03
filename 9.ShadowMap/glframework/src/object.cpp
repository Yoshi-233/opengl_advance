//
// Created by nio on 24-9-19.
//

#include "object.h"

Object::Object()
= default;

Object::~Object()
= default;

void Object::setPosition(glm::vec3 pos)
{
        this->mPosition = pos;
}

glm::vec3 Object::getPosition()
{
        return this->mPosition;
}


void Object::rotateX(float angle)
{
        this->mAngleX += angle;
}

void Object::rotateY(float angle)
{
        this->mAngleY += angle;
}

void Object::rotateZ(float angle)
{
        this->mAngleZ += angle;
}

void Object::setAngleX(float angle)
{
        this->mAngleX = angle;
}

void Object::setAngleY(float angle)
{
        this->mAngleY = angle;
}

void Object::setAngleZ(float angle)
{
        this->mAngleZ = angle;
}

void Object::setScale(glm::vec3 scale)
{
        this->mScale = scale;
}

glm::mat4 Object::getModelMatrix() const
{
        glm::mat4 parentModelMatrix{1.0f};
        if (this->mParent) {
                parentModelMatrix = this->mParent->getModelMatrix();
        }
        // unity下：先做缩放，旋转，平移
        glm::mat4 transform{1.0f};

        transform = glm::scale(transform, this->mScale);

        // unity旋转标准pitch yaw roll
        // 基于transform后的本地x轴
        transform = glm::rotate(transform, glm::radians(this->mAngleX),
                                glm::vec3(1.0, 0.0, 0.0));
        transform = glm::rotate(transform, glm::radians(this->mAngleY),
                                glm::vec3(0.0, 1.0, 0.0));
        transform = glm::rotate(transform, glm::radians(this->mAngleZ),
                                glm::vec3(0.0, 0.0, 1.0));

        // 平移，在世界坐标系下, mPosition是世界坐标系
        transform = parentModelMatrix * glm::translate(glm::mat4{1.0f}, this->mPosition) * transform;

        return transform;
}

glm::vec3 Object::getDirection() const
{
        auto modelMatrix = glm::mat3(this->getModelMatrix());
        return glm::normalize(-modelMatrix[2]);
}

void Object::addChild(const std::shared_ptr<Object>& child)
{
        // 1. 检查是否加入过
        auto iter = std::find(this->mChildren.begin(), this->mChildren.end(), child);
        if (iter!= this->mChildren.end()) {
                ERROR("Object::addChild: child already added");
                return;
        }
        // 2. 加入到children列表中
        this->mChildren.push_back(child);
        // 3. 设置child的parent
        child->mParent = this->shared_from_this();
}

std::vector<std::shared_ptr<Object>> Object::getChildren()
{
        return this->mChildren;
}

std::shared_ptr<Object> Object::getParent()
{
        return this->mParent;
}

ObjectType Object::getType() const
{
        return this->mType;
}



