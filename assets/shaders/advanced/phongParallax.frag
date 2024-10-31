#version 460 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;
in mat3 tbn;

uniform sampler2D sampler;	//diffuse��ͼ������
uniform sampler2D specularMaskSampler;//specularMask��ͼ������
uniform sampler2D normalMapSampler;
uniform sampler2D parallaxMapSampler;

uniform float heightScale;
uniform float layerNum;

uniform vec3 ambientColor;

//�������λ��
uniform vec3 cameraPosition;


uniform float shiness;

//͸����
uniform float opacity;


struct DirectionalLight{
	vec3 direction;
	vec3 color;
	float specularIntensity;
	float intensity;
};

struct PointLight{
	vec3 position;
	vec3 color;
	float specularIntensity;

	float k2;
	float k1;
	float kc;
};

struct SpotLight{
	vec3 position;
	vec3 targetDirection;
	vec3 color;
	float outerLine;
	float innerLine;
	float specularIntensity;
};

uniform DirectionalLight directionalLight;

//�������������
vec3 calculateDiffuse(vec3 lightColor, vec3 objectColor, vec3 lightDir, vec3 normal){
	float diffuse = clamp(dot(-lightDir, normal), 0.0,1.0);
	vec3 diffuseColor = lightColor * diffuse * objectColor;

	return diffuseColor;
}

//���㾵�淴�����
vec3 calculateSpecular(vec3 lightColor, vec3 lightDir, vec3 normal, vec3 viewDir, float intensity){
	//1 ��ֹ�����Ч��
	float dotResult = dot(-lightDir, normal);
	float flag = step(0.0, dotResult);
	vec3 lightReflect = normalize(reflect(lightDir,normal));

	//2 jisuan specular
	float specular = max(dot(lightReflect,-viewDir), 0.0);

	//3 ���ƹ�ߴ�С
	specular = pow(specular, shiness);

	//float specularMask = texture(specularMaskSampler, uv).r;

	//4 ����������ɫ
	vec3 specularColor = lightColor * specular * flag * intensity;

	return specularColor;
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir){
	//������յ�ͨ������
	vec3 objectColor  = texture(sampler, uv).xyz;
	vec3 lightDir = normalize(worldPosition - light.position);
	vec3 targetDir = normalize(light.targetDirection);

	//����spotlight�����䷶Χ
	float cGamma = dot(lightDir, targetDir);
	float intensity =clamp( (cGamma - light.outerLine) / (light.innerLine - light.outerLine), 0.0, 1.0);

	//1 ����diffuse
	vec3 diffuseColor = calculateDiffuse(light.color,objectColor, lightDir,normal);

	//2 ����specular
	vec3 specularColor = calculateSpecular(light.color, lightDir,normal, viewDir,light.specularIntensity); 

	return (diffuseColor + specularColor)*intensity;
}

vec3 calculateDirectionalLight(vec3 objectColor, DirectionalLight light, vec3 normal ,vec3 viewDir){
	light.color *= light.intensity;

	//������յ�ͨ������
	vec3 lightDir = normalize(light.direction);

	//1 ����diffuse
	vec3 diffuseColor = calculateDiffuse(light.color,objectColor, lightDir,normal);

	//2 ����specular
	vec3 specularColor = calculateSpecular(light.color, lightDir,normal, viewDir,light.specularIntensity); 

	return diffuseColor + specularColor;
}

vec3 calculatePointLight(vec3 objectColor, PointLight light, vec3 normal ,vec3 viewDir){
	//������յ�ͨ������
	vec3 lightDir = normalize(worldPosition - light.position);

	//����˥��
	float dist = length(worldPosition - light.position);
	float attenuation = 1.0 / (light.k2 * dist * dist + light.k1 * dist + light.kc);

	//1 ����diffuse
	vec3 diffuseColor = calculateDiffuse(light.color,objectColor, lightDir,normal);

	//2 ����specular
	vec3 specularColor = calculateSpecular(light.color, lightDir,normal, viewDir,light.specularIntensity); 

	return (diffuseColor + specularColor)*attenuation;
}

//uv����ǰ���ص�ԭʼuv
//viewDir����ǰ���������֮������߷�����������ϵ�ռ���
vec2 parallaxUV(vec2 uv, vec3 viewDir){
	viewDir = normalize(transpose(tbn) * viewDir);
	
	//1 ׼���������� 
	float layerDepth = 1.0 / layerNum;//ÿһ���ж���
	vec2 deltaUV = viewDir.xy / viewDir.z * heightScale / layerNum;

	//2 ��ʼ��
	vec2 currentUV = uv;
	float currentSampleValue = texture(parallaxMapSampler,currentUV).r;
	float currentLayerDepth = 0.0;

	//3 ѭ������
	while(currentSampleValue > currentLayerDepth){
		currentUV -= deltaUV;
		currentSampleValue = texture(parallaxMapSampler,currentUV).r;
		currentLayerDepth += layerDepth;
	}

	vec2 lastUV = currentUV + deltaUV;
	//����lastLen
	float lastLayerDepth = currentLayerDepth - layerDepth;
	float lastHeight = texture(parallaxMapSampler,lastUV).r;
	float lastLen = lastHeight - lastLayerDepth;
	//����currentLen
	float currLen = currentLayerDepth - currentSampleValue;

	float lastWeight = currLen / (currLen + lastLen);

	vec2 targetUV = mix(currentUV, lastUV, lastWeight);

	return targetUV;
}

void main()
{
	vec3 viewDir = normalize(worldPosition - cameraPosition);
	vec2 uvReal = parallaxUV(uv, viewDir);

//���������
	vec3 objectColor  = texture(sampler, uvReal).xyz ;
	vec3 result = vec3(0.0,0.0,0.0);

	//������յ�ͨ������
	vec3 normalN = texture(normalMapSampler,uvReal).rgb;
	normalN = normalN * 2.0 - vec3(1.0); //��0-1���-1~1
	normalN = normalize(tbn * normalN);


	result += calculateDirectionalLight(objectColor, directionalLight,normalN, viewDir);

	
	float alpha =  texture(sampler, uvReal).a;

	vec3 ambientColor = objectColor * ambientColor;

	vec3 finalColor = result + ambientColor;
	

	FragColor = vec4(finalColor,alpha * opacity);
}