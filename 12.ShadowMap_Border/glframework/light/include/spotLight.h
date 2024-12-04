//
// Created by Shaozheming on 2024/9/21.
//

#ifndef SPOTLIGHT_SPOTLIGHT_H
#define SPOTLIGHT_SPOTLIGHT_H

#include "light.h"
#include "../../include/object.h"

class SpotLight : public Light {
public:
        SpotLight();

        ~SpotLight() override = default;

        float getInnerAngle() const;

        void setInnerAngle(float innerAngle);

        float getOuterAngle() const;

        void setOuterAngle(float outerAngle);

private:
        float mInnerAngle{0.0f};
        float mOuterAngle{0.0f};
};

#endif //SPOTLIGHT_SPOTLIGHT_H
