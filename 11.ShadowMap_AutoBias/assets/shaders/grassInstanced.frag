#version 410 core
out vec4 fragColor;

uniform sampler2D sampler;
uniform sampler2D specularMaskSampler;
uniform sampler2D opacityMask;
uniform sampler2D cloudMask;

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

// 环境光
uniform vec3 ambientColor;

// 相机世界位置
uniform vec3 cameraPosition;

// 高光强度，由物体材质决定
uniform float shiness;

// 草地贴图特性
uniform float uvScale;
uniform float brightness;

// 云层
uniform vec3 cloudWhiteColor;
uniform vec3 cloudBlackColor;
uniform float cloudUVScale;
uniform float time;
uniform float cloudSpeed;
uniform vec3 windDirection;
uniform float cloudLerp;

in vec2 worldXZ;

struct DirectionalLight {
        vec3 direction;
        vec3 color;
        float specularIntensity;
        float intensity;
};

struct PointLight {
        vec3 position;
        vec3 color;
        float specularIntensity;

        float kc;
        float k1;
        float k2;
};

struct SpotLight {
        vec3 position;
        vec3 targetDirection;
        vec3 color;
        float innerLine;
        float outerLine;
        float specularIntensity;
};

#define POINT_LIGHT_NUM 4

uniform SpotLight spotLight;
uniform PointLight pointLights[POINT_LIGHT_NUM];
uniform DirectionalLight directionalLight;
uniform float specularMaskFlag = 1.0f;
uniform float pointLightsFlag = 1.0f;
uniform float spotLightFlag = 1.0f;

/* 透明度 */
uniform float opacity = 1.0f;

// 计算漫反射相关的函数
vec3 calculateDiffuse(vec3 lightColor, vec3 objColor, vec3 lightDirN, vec3 normalN)
{
        /* 需要lightDir和normal都进行归一化 */
        float diffuse = clamp(dot(-lightDirN, normalN), 0.0f, 1.0f);// cos夹角
        vec3 diffuseColor = lightColor * diffuse * objColor;

        return diffuseColor;
}

// 计算镜面反射相关的函数
vec3 calculateSpecular(vec3 lightColor, vec3 lightDirN, vec3 normalN, vec3 viewDir, float intensity)
{
        /* 需要lightDir和normal都进行归一化 */
        // 1. 防止背面光的照入
        float dotResult = dot(-lightDirN, normalN);
        float specularFlag = step(0.0f, dotResult);// 大于0返回1，否则返回0，类似if

        // 2. 计算镜面反射
        vec3 lightReflect = normalize(reflect(lightDirN, normalN));
        float specular = max(dot(lightReflect, -viewDir), 0.0f);

        // 3. 计算高光强度
        specular = pow(specular, shiness);
        // 高光蒙板
        float specularMask = (specularMaskFlag == 1) ? texture(specularMaskSampler, uv).r : 1;

        // 这里并不需要objColor，光经过物体反射出去了，漫反射是物体吸收
        // specularIntensity控制光斑呈现的亮度
        vec3 specularColor = lightColor * specular * specularFlag * intensity * specularMask;

        return specularColor;
}

// 计算聚光灯相关的函数
vec3 calculateSpotLight(vec3 objColor, SpotLight light, vec3 normalN, vec3 viewDir)
{
        /* 需要lightDir和normal都进行归一化 */
        /* 1. 计算光照的通用数据 */
        vec3 lightDirN = normalize(worldPosition - light.position);
        // 摄像机到物体向量 数据
        vec3 targetDirN = normalize(light.targetDirection);

        /* 2.计算spotLight的照射范围 */
        float cGamma = dot(lightDirN, targetDirN);
        float intensity = clamp((cGamma - light.outerLine) / (light.innerLine - light.outerLine), 0.0f, 1.0f);

        /* 3. 准备漫反射相关数据 */
        vec3 diffuseColor = calculateDiffuse(light.color, objColor, lightDirN, normalN);

        /* 4. 计算specular */
        vec3 specularColor = calculateSpecular(light.color, lightDirN, normalN, viewDir, light.specularIntensity);

        return (diffuseColor + specularColor) * intensity;
}

// 计算点光源相关的函数
vec3 calculatePointLight(vec3 objColor, PointLight light, vec3 normalN, vec3 viewDir)
{
        /* 1. 计算光照的通用数据 */
        vec3 lightDirN = normalize(worldPosition - light.position);

        /* 2. 准备漫反射相关数据 */
        vec3 diffuseColor = calculateDiffuse(light.color, objColor, lightDirN, normalN);

        /* 3. 计算specular */
        // 计算衰减值
        float dist = length(worldPosition - light.position);
        float attenuation = 1.0 / (light.kc + light.k1 * dist + light.k2 * dist * dist);
        vec3 specularColor = calculateSpecular(light.color, lightDirN, normalN, viewDir, light.specularIntensity);

        return (diffuseColor + specularColor) * attenuation;
}

// 计算平行光相关的函数
vec3 calculateDirectionalLight(vec3 objColor, DirectionalLight light, vec3 normalN, vec3 viewDir)
{
        light.color *= light.intensity;
        /* 1. 计算光照的通用数据 */
        vec3 lightDirN = normalize(light.direction);

        /* 2. 准备漫反射相关数据 */
        vec3 diffuseColor = calculateDiffuse(light.color, objColor, lightDirN, normalN);

        /* 3. 计算specular */
        vec3 specularColor = calculateSpecular(light.color, lightDirN, normalN, viewDir, light.specularIntensity);

        return diffuseColor + specularColor;

}

void main()
{
        vec3 result = vec3(0.0f, 0.0f, 0.0f);
        vec2 worldUV = worldXZ / uvScale;

        // 计算光照的通用数据
        vec3 objColor = texture(sampler, worldUV).rgb * brightness;//物体颜色
        float objAlpha = texture(opacityMask, uv).r;
        if(objAlpha == 0.0f) {
                discard;
        }
        // diffuse 数据
        vec3 normalN = normalize(normal);
        vec3 lightDirN = normalize(worldPosition - spotLight.position);
        // 摄像机到物体向量 数据
        vec3 viewDir = normalize(worldPosition - cameraPosition);
        vec3 targetDirN = normalize(spotLight.targetDirection);

        /* 计算各种光照 */
        result += (spotLightFlag != 0.0f) ? calculateSpotLight(objColor, spotLight, normalN, viewDir) : vec3(0.0f);
        result += calculateDirectionalLight(objColor, directionalLight, normalN, viewDir);
        for (int i = 0; i < POINT_LIGHT_NUM; i++) {
                result += (pointLightsFlag != 0.0f) ?
                calculatePointLight(objColor, pointLights[i], normalN, viewDir) : vec3(0.0f);
        }

        /* 环境光 */
        vec3 ambientColor = objColor * ambientColor;

        /* 计算最终颜色 */
        vec3 grassColor = result + ambientColor;

        /*   云层   */
        vec3 windDirN = normalize(windDirection);
        vec2 cloudUV = worldXZ / cloudUVScale;
        cloudUV += time * cloudSpeed * windDirN.xz;
        float cloudMask = texture(cloudMask, cloudUV).r;
        vec3 cloudColor = mix(cloudBlackColor, cloudWhiteColor, cloudMask);

        vec3 finalColor = mix(grassColor, cloudColor, cloudLerp);

        // 最终计算
        fragColor = vec4(finalColor, objAlpha * opacity);
}