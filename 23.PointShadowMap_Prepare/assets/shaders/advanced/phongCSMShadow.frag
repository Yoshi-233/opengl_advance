#version 410 core
out vec4 fragColor;

uniform sampler2D sampler;
uniform sampler2D specularMaskSampler;
uniform sampler2DArray shadowMapSampler;
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

uniform float bias;// 给美术调节 shadowMap 阴影判定的偏移量

#define NUM_SAMPLES 32
#define PI 3.141592653589793
#define PI2 6.283185307179586

float rand_2to1(vec2 uv)
{
        const highp float a = 12.9898;
        const highp float b = 78.233;
        const highp float c = 43758.5453;
        highp float dt= dot(uv.xy, vec2(a, b));
        highp float sn= mod(dt, PI);
        return fract(sin(sn) * c);
}

uniform float diskTightness;
vec2 disk[NUM_SAMPLES];

void poissonDiskSamples(vec2 randomSeed)
{
        /* 1.init 弧度 */
        float angle = rand_2to1(randomSeed) * PI2;

        /* 2.init r */
        float radius = 1.0f / float(NUM_SAMPLES);

        /* 3.弧度步长 */
        float angleStep = 3.88322077450933;// 黄金分割

        /* 4.半径步长 */
        float radiusStep = radius;

        /* 5.循环生成 */
        for (int i = 0; i < NUM_SAMPLES; i++) {
                disk[i] = vec2(cos(angle), sin(angle)) * pow(radius, diskTightness);
                angle += angleStep;
                radius += radiusStep;
        }
}

float getBias(vec3 normal, vec3 lightDir)
{
        vec3 normalN = normalize(normal);
        vec3 lightDirN = normalize(lightDir);

        return max(bias * (1.0f - dot(normalN, lightDirN)), 0.0005f);
}

//float calculateShadow(vec3 normal, vec3 lightDir, vec4 lightSpaceClipCoord)
//{
//        /* 1. 找到当前像素在光源空间中的NDC位置 */
//        vec3 lightNDC = lightSpaceClipCoord.xyz / lightSpaceClipCoord.w;
//
//        /* 2. 当前像素在shadwmap中的uv坐标 */
//        vec3 projCoords = lightNDC.xyz * 0.5 + 0.5;
//        vec2 uvCoords = projCoords.xy;
//
//        /* 3. 读取shadowmap采样，closeDepth */
//        float closeDepth = texture(shadowMapSampler, uvCoords).r;
//
//        /* 4. 计算当前像素的深度值, 对比closeDepth */
//        float selfDepth = projCoords.z;
//
//        // 有可能发生自遮挡
//        float shadow = (selfDepth - getBias(normal, lightDir)) > closeDepth ? 1.0f : 0.0f;
//
//        return shadow;
//}

//float pcf(vec3 normal, vec3 lightDir, vec4 lightSpaceClipCoord)
//{
//        /* 1. 找到当前像素在光源空间中的NDC位置 */
//        vec3 lightNDC = lightSpaceClipCoord.xyz / lightSpaceClipCoord.w;
//
//        /* 2. 当前像素在shadwmap中的uv坐标 */
//        vec3 projCoords = lightNDC.xyz * 0.5 + 0.5;
//        // 获取当前像素的uv长度，xy轴都有, 0表示第0层mipmap
//        vec2 texelSize = 1.0 / textureSize(shadowMapSampler, 0);
//
//        /* 3. 读取shadowmap采样，closeDepth */
//        float sum = 0.0f;
//        for(int x = -1; x <= 1; ++x) {
//                for(int y = -1; y <= 1; ++y) {
//                        float closeDepth = texture(shadowMapSampler,
//                                projCoords.xy + vec2(x, y) * texelSize).r;
//                        float selfDepth = projCoords.z;
//                        /* 4. 计算当前像素的深度值, 对比closeDepth */
//                        sum += (selfDepth - getBias(normal, lightDir)) > closeDepth ? 1.0f : 0.0f;
//                }
//        }
//
//        return sum / 9.0f;
//}

uniform float pcfRadius;//全局模糊范围，如果是1.0f，则没有全局模糊， 因为全被平均了
float pcf_poisson(vec3 normal, vec3 lightDir, vec4 lightSpaceClipCoord, float pcfUVRadius, int layer)
{
        /* 1. 找到当前像素在光源空间中的NDC位置 */
        vec3 lightNDC = lightSpaceClipCoord.xyz / lightSpaceClipCoord.w;

        /* 2. 当前像素在shadwmap中的uv坐标 */
        vec3 projCoords = lightNDC.xyz * 0.5 + 0.5;
        poissonDiskSamples(projCoords.xy);

        /* 3. 读取shadowmap采样，closeDepth */
        float sum = 0.0f;
        for (int i = 0; i < NUM_SAMPLES; i++) {
                float closeDepth = texture(shadowMapSampler,
                vec3(projCoords.xy + disk[i] * pcfUVRadius, layer)).r;
                float selfDepth = projCoords.z;
                sum += (selfDepth - getBias(normal, lightDir)) > closeDepth ? 1.0f : 0.0f;
        }

        return sum / float(NUM_SAMPLES);
}

// PCSS param
//uniform float lightSize; // 光源大小
//uniform float frustum; // 视锥体平面大小
//uniform float nearPlane; // 正交投影的near
//
//float findBlocker(vec3 lightSpacePosition, vec2 shadowUV,
//                float depthReceiver, vec3 normal, vec3 lightDir)
//{
//        poissonDiskSamples(shadowUV);
//
//        float searchRadius = ((-lightSpacePosition.z - nearPlane) /
//                            -lightSpacePosition.z) * lightSize;
//        float searchRadiusUV = searchRadius / frustum;
//
//        float blockerNum = 0.0f;
//        float blockerSumDepth = 0.0f;
//        for(int i = 0; i < NUM_SAMPLES; i++) {
//                float samplerDepth = texture(shadowMapSampler, shadowUV +
//                                        disk[i] * searchRadiusUV).r;
//                if(samplerDepth < depthReceiver - getBias(normal, lightDir)) {
//                        blockerNum++;
//                        blockerSumDepth += samplerDepth;
//                }
//        }
//
//        return blockerNum != 0.0f ? blockerSumDepth / blockerNum : -1.0f;
//}
//
//float pcss(vec3 lightSpacePosition,  vec4 lightSpaceClipCoord,  vec3 normal, vec3 lightDir)
//{
//        /* 1. 找到当前像素在光源空间中的NDC位置 */
//        vec3 lightNDC = lightSpaceClipCoord.xyz / lightSpaceClipCoord.w;
//
//        /* 2. 当前像素在shadwmap中的uv坐标 */
//        vec3 projCoords = lightNDC.xyz * 0.5 + 0.5;
//        vec2 uv = projCoords.xy;
//        float depth = projCoords.z;
//
//        /* 3.计算blocker */
//        float dBlocker = findBlocker(lightSpacePosition, uv, depth, normal, -directionalLight.direction);
//
//        /* 4.penumba */
//        float penumba = ((depth - dBlocker) / dBlocker) * lightSize / frustum;
//
//        /* 5.计算真正的pcfRadius */
//        return pcf_poisson(normal, lightDir, lightSpaceClipCoord, penumba * pcfRadius);
//}

uniform int csmLayerCount;
uniform float csmLayers[20];
uniform mat4 lightMatrices[20];
uniform mat4 viewMatrix;// 和vertex相同
int getCurrentLayer(vec3 positionWorldSpace)
{
        /* 1. 计算当前camera系的坐标 */
        vec3 positionCameraSpace = (viewMatrix * vec4(positionWorldSpace, 1.0f)).xyz;
        float z = -positionCameraSpace.z;

        /* 2. 计算当前层级 */
        int layer = 0;
        // csmLayerCount个层级， 0-csmLayerCount
        for (int i = 0; i <= csmLayerCount; i++) {
                if (z < csmLayers[i]) {
                        layer = i - 1;
                        break;
                }
        }
        return layer;
}

float csm(vec3 positionWorldSpace, vec3 normal, vec3 lightDir, float pcfRadius)
{
        int layer = getCurrentLayer(positionWorldSpace);
        vec4 lightSpaceClipCoord = lightMatrices[layer] * vec4(positionWorldSpace, 1.0f);

        return pcf_poisson(normal, lightDir, lightSpaceClipCoord, pcfRadius, layer);
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
        // shadow = pcss(lightSpacePosition, lightSpaceClipCoord, normal, -directionalLight.direction);
        shadow = csm(worldPosition, normal, -directionalLight.direction, pcfRadius);

        /* 计算最终颜色 */
        vec3 finalColor = result * (1.0f - shadow) + ambientColor;

        // 最终计算
        fragColor = vec4(finalColor, objAlpha * opacity);
}