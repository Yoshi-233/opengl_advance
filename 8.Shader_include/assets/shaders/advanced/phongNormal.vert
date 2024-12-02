#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

out vec2 uv;
out vec3 normal;
out vec3 worldPosition;
out mat3 tbn;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

void main()
{
        vec4 transformPosition = vec4(aPos, 1.0f);
        transformPosition = modelMatrix * transformPosition;// 1.0表示确切位置
        worldPosition = transformPosition.xyz;

        gl_Position = projectionMatrix * viewMatrix * transformPosition;
        uv = aUV;

        // 当前使用平面比较简单，法线经过TBN后不变，使用默认的法线即可
        normal = normalMatrix * aNormal;

        vec3 tangent = normalize(mat3(modelMatrix) * aTangent);
        vec3 bitangent = normalize(cross(normal, tangent));

        tbn = mat3(tangent, bitangent, normal);
}