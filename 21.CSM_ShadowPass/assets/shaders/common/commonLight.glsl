struct DirectionalLight {
        vec3 direction;
        vec3 color;
        float specularIntensity;
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
vec3 calculateSpotLight(SpotLight light, vec3 normalN, vec3 viewDir)
{
        /* 需要lightDir和normal都进行归一化 */
        /* 1. 计算光照的通用数据 */
        vec3 objColor = texture(sampler, uv).rgb;//物体颜色
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
vec3 calculatePointLight(PointLight light, vec3 normalN, vec3 viewDir)
{
        /* 1. 计算光照的通用数据 */
        vec3 objColor = texture(sampler, uv).rgb;//物体颜色
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
vec3 calculateDirectionalLight(DirectionalLight light, vec3 normalN, vec3 viewDir)
{
        /* 1. 计算光照的通用数据 */
        vec3 objColor = texture(sampler, uv).rgb;//物体颜色
        vec3 lightDirN = normalize(light.direction);

        /* 2. 准备漫反射相关数据 */
        vec3 diffuseColor = calculateDiffuse(light.color, objColor, lightDirN, normalN);

        /* 3. 计算specular */
        vec3 specularColor = calculateSpecular(light.color, lightDirN, normalN, viewDir, light.specularIntensity);

        return diffuseColor + specularColor;

}