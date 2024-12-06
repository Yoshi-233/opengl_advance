#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in mat4 aInstanceMatrix;

out vec2 uv;
out vec3 normal;
out vec3 worldPosition;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

void main()
{
        vec4 transformPosition = vec4(aPos, 1.0f);

        // 每一次都先x自己的变换，再x总体的变换
        transformPosition = modelMatrix * aInstanceMatrix * transformPosition;// 1.0表示确切位置
        worldPosition = transformPosition.xyz;

        // aPos不允许更改
        // transform * vec4(aPos, 1.0)是自己旋转
        // viewMatrix是摄像机
        // projectionMatrix是投影矩阵
        gl_Position = projectionMatrix * viewMatrix * transformPosition;
        uv = aUV;

        normal = normalMatrix * aNormal;
}