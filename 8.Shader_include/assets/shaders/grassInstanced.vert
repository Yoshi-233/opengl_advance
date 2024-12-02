#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aColor;
layout (location = 4) in mat4 aInstanceMatrix;

out vec2 uv;
out vec3 normal;
out vec3 worldPosition;
out vec2 worldXZ;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

uniform float time;

// 风力
uniform vec3 windDirection;
uniform float windStrength;
uniform float phaseScale;

void main()
{
        vec4 transformPosition = vec4(aPos, 1.0f);

        // 每一次都先x自己的变换，再x总体的变换
        transformPosition = modelMatrix * aInstanceMatrix * transformPosition;// 1.0表示确切位置
        worldXZ -= transformPosition.xz;

        // 风力变动
        vec3 windDirN = normalize(windDirection);
        float phaseDistance = dot(windDirN, transformPosition.xyz);
        transformPosition += vec4(sin(time  +  phaseDistance / phaseScale) *
                (1.0 - aColor.r) * windStrength * windDirN, 0.0);
        worldPosition = transformPosition.xyz;

        // aPos不允许更改
        // transform * vec4(aPos, 1.0)是自己旋转
        // viewMatrix是摄像机
        // projectionMatrix是投影矩阵
        gl_Position = projectionMatrix * viewMatrix * transformPosition;
        uv = aUV;

        normal = transpose(inverse(mat3(modelMatrix * aInstanceMatrix))) * aNormal;
}