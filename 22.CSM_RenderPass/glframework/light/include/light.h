//
// Created by Shaozheming on 2024/9/19.
//

#ifndef RENDERER_LIGHT_H
#define RENDERER_LIGHT_H

#include "../../include/core.h"
#include "../../include/object.h"
#include "../shadow/include/shadow.h"

class Light : public Object {
public:
        Light() = default;

        ~Light() override = default;

        [[nodiscard]] glm::vec3 getColor() const;

        void setColor(const glm::vec3& color);

        [[nodiscard]] float getSpecularIntensity() const ;

        void setSpecularIntensity(float intensity);

        [[nodiscard]] float getIntensity() const;

        void setIntensity(float intensity);
public:
        float mIntensity {1.0f};

protected:
        glm::vec3 mColor {1.0f};
        float mSpecularIntensity {1.0f};

public:
        std::shared_ptr<Shadow> mShadow;

};

#endif //RENDERER_LIGHT_H
