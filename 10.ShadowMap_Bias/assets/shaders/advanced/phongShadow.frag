#version 410 core
out vec4 fragColor;

uniform sampler2D sampler;
uniform sampler2D specularMaskSampler;
uniform sampler2D shadowMapSampler;
in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

// 环境光
uniform vec3 ambientColor;

// 相机世界位置
uniform vec3 cameraPosition;

// 高光强度，由物体材质决定
uniform float shiness;

#define POINT_LIGHT_NUM 4

uniform float specularMaskFlag = 1.0f;
uniform float pointLightsFlag = 1.0f;
uniform float spotLightFlag = 1.0f;

/* 透明度 */
uniform float opacity = 1.0f;

#include "../common/commonLight.glsl"

uniform SpotLight spotLight;
uniform PointLight pointLights[POINT_LIGHT_NUM];
uniform DirectionalLight directionalLight;

in vec4 lightSpaceClipCoord;// 光源空间内的裁剪坐标
uniform float bias; // 给美术调节 shadowMap 阴影判定的偏移量

float calculateShadow(vec4 lightSpaceClipCoord)
{
        /* 1. 找到当前像素在光源空间中的NDC位置 */
        vec3 lightNDC = lightSpaceClipCoord.xyz / lightSpaceClipCoord.w;

        /* 2. 当前像素在shadwmap中的uv坐标 */
        vec3 projCoords = lightNDC.xyz * 0.5 + 0.5;
        vec2 uvCoords = projCoords.xy;

        /* 3. 读取shadowmap采样，closeDepth */
        float closeDepth = texture(shadowMapSampler, uvCoords).r;

        /* 4. 计算当前像素的深度值, 对比closeDepth */
        float selfDepth = projCoords.z;

        // 有可能发生自遮挡
        float shadow = (selfDepth - bias) > closeDepth ? 1.0f : 0.0f;

        return shadow;
}

void main()
{
        vec3 result = vec3(0.0f, 0.0f, 0.0f);
        // 计算光照的通用数据
        vec3 objColor = texture(sampler, uv).rgb;//物体颜色
        float objAlpha = texture(sampler, uv).a;
        // diffuse 数据
        vec3 normalN = normalize(normal);
        vec3 lightDirN = normalize(worldPosition - spotLight.position);
        // 摄像机到物体向量 数据
        vec3 viewDir = normalize(worldPosition - cameraPosition);
        vec3 targetDirN = normalize(spotLight.targetDirection);

        /* 计算各种光照 */
        result += (spotLightFlag != 0.0f) ? calculateSpotLight(spotLight, normalN, viewDir) : vec3(0.0f);
        result += calculateDirectionalLight(directionalLight, normalN, viewDir);
        for (int i = 0; i < POINT_LIGHT_NUM; i++) {
                result += (pointLightsFlag != 0.0f) ?
                calculatePointLight(pointLights[i], normalN, viewDir) : vec3(0.0f);
        }

        /* 环境光 */
        vec3 ambientColor = objColor * ambientColor;

        /* 计算shadow */
        float shadow = 1.0f;
        shadow = calculateShadow(lightSpaceClipCoord);

        /* 计算最终颜色 */
        vec3 finalColor = result * (1.0f - shadow) + ambientColor;

        // 最终计算
        fragColor = vec4(finalColor, objAlpha * opacity);
}