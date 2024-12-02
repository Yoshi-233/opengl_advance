#version 410 core
layout (location = 0) in vec3 aPos;

out vec3 uvw;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
        vec4 transformPosition = vec4(aPos, 1.0f);
        transformPosition = modelMatrix * transformPosition;// 1.0表示确切位置

        gl_Position = projectionMatrix * viewMatrix * transformPosition;
        gl_Position = gl_Position.xyww; // 透视除法， 这样保证天空盒深度最深
        uvw = aPos;
}