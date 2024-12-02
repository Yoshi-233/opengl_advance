#version 410 core
out vec4 fragColor;

in vec3 uvw;

uniform sampler2D diffuse;
uniform samplerCube cubeSampler;
uniform sampler2D sphericalSampler;

const float PI = 3.14159265359;

vec2 uvwToYv(vec3 uvwN)
{
        float phi = asin(uvwN.y);
        float theta = atan(uvwN.z, uvwN.x);
        float u = theta / (2 * PI) + 0.5;
        float v = phi / PI + 0.5;
        return vec2(u, v);
}

void main()
{
        fragColor = texture(sphericalSampler, uvwToYv(normalize(uvw)));
}