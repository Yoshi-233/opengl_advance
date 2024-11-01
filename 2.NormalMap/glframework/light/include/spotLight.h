//
// Created by Shaozheming on 2024/9/21.
//

#ifndef SPOTLIGHT_SPOTLIGHT_H
#define SPOTLIGHT_SPOTLIGHT_H

#include "light.h"
#include "../../include/object.h"

class SpotLight : public Light, public Object {
public:
        SpotLight();

        virtual ~SpotLight() override = default;

        const glm::vec3 &getTargetDirection() const;

        void setTargetDirection(const glm::vec3 &targetDirection);

        float getInnerAngle() const;

        void setInnerAngle(float innerAngle);

        float getOuterAngle() const;

        void setOuterAngle(float outerAngle);

private:
        glm::vec3 mTargetDirection{-1.0f};
        float mInnerAngle{0.0f};
        float mOuterAngle{0.0f};
};

#endif //SPOTLIGHT_SPOTLIGHT_H
