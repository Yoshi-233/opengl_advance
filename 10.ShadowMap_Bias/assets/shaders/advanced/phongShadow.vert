#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;

out vec2 uv;
out vec3 normal;
out vec3 worldPosition;
out vec4 lightSpaceClipCoord;// 光源空间内的裁剪坐标

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

uniform mat4 lightMatrix;

void main()
{
        vec4 transformPosition = vec4(aPos, 1.0f);
        transformPosition = modelMatrix * transformPosition;// 1.0表示确切位置
        worldPosition = transformPosition.xyz;

        // aPos不允许更改
        // transform * vec4(aPos, 1.0)是自己旋转
        // viewMatrix是摄像机
        // projectionMatrix是投影矩阵
        gl_Position = projectionMatrix * viewMatrix * transformPosition;
        uv = aUV;

        normal = normalMatrix * aNormal;

        lightSpaceClipCoord = lightMatrix * transformPosition;
}