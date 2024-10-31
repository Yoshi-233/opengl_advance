#version 410 core
out vec4 fragColor;

in vec2 uv;
in vec3 normal;

// 线性深度值相关参数
uniform float near;
uniform float far;

void main()
{
        float Zndc = gl_FragCoord.z * 2.0f - 1.0f;
        float linearDepth = 2.0f * near / (far + near - Zndc * (far - near));
        fragColor = vec4(linearDepth, linearDepth, linearDepth, 1.0);
}