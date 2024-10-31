#version 460 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;
in vec4 lightSpaceClipCoord;
in vec3 lightSpacePosition;

uniform sampler2D sampler;	//diffuse��ͼ������
uniform sampler2D specularMaskSampler;//specularMask��ͼ������

uniform sampler2D shadowMapSampler;
uniform float bias;

uniform vec3 ambientColor;

//�������λ��
uniform vec3 cameraPosition;


uniform float shiness;

//͸����
uniform float opacity;


#include "../common/commonLight.glsl"

uniform DirectionalLight directionalLight;

#define NUM_SAMPLES 32
#define PI 3.141592653589793
#define PI2 6.283185307179586

float rand_2to1(vec2 uv ) { 
  // 0 - 1
	const highp float a = 12.9898, b = 78.233, c = 43758.5453;
	highp float dt = dot( uv.xy, vec2( a,b ) ), sn = mod( dt, PI );
	return fract(sin(sn) * c);
}

uniform float diskTightness;
vec2 disk[NUM_SAMPLES];
void poissonDiskSamples(vec2 randomSeed){
	//1 ��ʼ����
	float angle = rand_2to1(randomSeed) * PI2;

	//2 ��ʼ�뾶
	float radius = 1.0 / float(NUM_SAMPLES);

	//3 ���Ȳ���
	float angleStep = 3.883222077450933;

	//4 �뾶����
	float radiusStep = radius;

	//5 ѭ������
	for(int i = 0;i < NUM_SAMPLES;i++){
		disk[i] = vec2(cos(angle), sin(angle)) * pow(radius, diskTightness);
		radius += radiusStep;
		angle += angleStep;
	}
}



float getBias(vec3 normal, vec3 lightDir){
	vec3 normalN = normalize(normal);
	vec3 lightDirN = normalize(lightDir);
	
	return max(bias * (1.0 - dot(normalN, lightDirN)), 0.0005);
}

float calculateShadow(vec3 normal, vec3 lightDir){
	//1 �ҵ���ǰ�����ڹ�Դ�ռ��ڵ�NDC����
	vec3 lightNDC = lightSpaceClipCoord.xyz/lightSpaceClipCoord.w;

	//2 �ҵ���ǰ������ShadowMap�ϵ�uv
	vec3 projCoord = lightNDC * 0.5 + 0.5;
	vec2 uv = projCoord.xy;

	//3 ʹ�����uv��ShadowMap���в������õ�ClosestDepth
	float closestDepth = texture(shadowMapSampler, uv).r;

	//4 �Աȵ�ǰ�����ڹ�Դ�ռ��ڵ����ֵ��ClosestDepth�Ĵ�С
	float selfDepth = projCoord.z;

	float shadow = (selfDepth - getBias(normal, lightDir)) > closestDepth? 1.0:0.0;

	return shadow;
}

//float pcf(vec3 normal, vec3 lightDir){
//	//1 �ҵ���ǰ�����ڹ�Դ�ռ��ڵ�NDC����
//	vec3 lightNDC = lightSpaceClipCoord.xyz/lightSpaceClipCoord.w;
//
//	//2 �ҵ���ǰ������ShadowMap�ϵ�uv
//	vec3 projCoord = lightNDC * 0.5 + 0.5;
//	vec2 uv = projCoord.xy;
//	float depth = projCoord.z;
//
//	vec2 texelSize = 1.0 / textureSize(shadowMapSampler, 0);
//
//	//3  �����Ź���ÿһ�������ֵ����Ҫ�뵱ǰ�����ڹ�Դ�µ����ֵ���жԱ�
//	float sum = 0.0;
//	for(int x = -1;x <= 1;x++){
//		for(int y = -1;y <= 1;y++){
//			float closestDepth = texture(shadowMapSampler,uv + vec2(x,y) * texelSize).r;
//			sum += closestDepth < (depth - getBias(normal, lightDir))?1.0:0.0;
//		}
//	}
//	return sum / 9.0;
//}

uniform float pcfRadius;
float pcf(vec3 normal, vec3 lightDir,float pcfUVRadius){
	//1 �ҵ���ǰ�����ڹ�Դ�ռ��ڵ�NDC����
	vec3 lightNDC = lightSpaceClipCoord.xyz/lightSpaceClipCoord.w;

	//2 �ҵ���ǰ������ShadowMap�ϵ�uv
	vec3 projCoord = lightNDC * 0.5 + 0.5;
	vec2 uv = projCoord.xy;
	float depth = projCoord.z;

	poissonDiskSamples(uv);

	vec2 texelSize = 1.0 / textureSize(shadowMapSampler, 0);

	//3  ����poisson�����̵�ÿһ�������㣬ÿһ�������ֵ����Ҫ�뵱ǰ�����ڹ�Դ�µ����ֵ���жԱ�
	float sum = 0.0;
	for(int i = 0;i < NUM_SAMPLES;i++){
		float closestDepth = texture(shadowMapSampler,uv + disk[i] * pcfUVRadius).r;
		sum += closestDepth < (depth - getBias(normal, lightDir))?1.0:0.0;
	}

	return sum / float(NUM_SAMPLES);
}

//PCSS ��ز��� 
uniform float lightSize;
uniform float frustum;
uniform float nearPlane;

float findBlocker(vec3 lightSpacePosition, vec2 shadowUV, float depthReceiver, vec3 normal,vec3 lightDir){
	poissonDiskSamples(shadowUV);
	
	float searchRadius = (-lightSpacePosition.z - nearPlane) / (-lightSpacePosition.z)*lightSize;
	float searchRadiusUV = searchRadius / frustum;

	float blockerNum = 0.0;
	float blockerSumDepth = 0.0;
	for(int i = 0;i < NUM_SAMPLES;i++){
		float sampleDepth = texture(shadowMapSampler, shadowUV + disk[i] * searchRadiusUV).r;
		if(depthReceiver - getBias(normal, lightDir) > sampleDepth){
			blockerNum += 1.0;
			blockerSumDepth += sampleDepth;
		}
	}

	return blockerNum != 0.0?blockerSumDepth / blockerNum:-1.0;
}

float pcss(vec3 lightSpacePosition, vec4 lightSpaceClipCoord, vec3 normal, vec3 lightDir) {
	//1 ��ȡ��ǰ������ShadowMap�µ�uv�Լ��ڹ�Դ����ϵ�µ����ֵ
	vec3 lightNDC = lightSpaceClipCoord.xyz/lightSpaceClipCoord.w;
	vec3 projCoord = lightNDC * 0.5 + 0.5;
	vec2 uv = projCoord.xy;
	float depth = projCoord.z;

	//2 ����dBlocker
	float dBlocker = findBlocker(lightSpacePosition, uv, depth, normal ,lightDir);
	
	//3 ����penumbra
	float penumbra = ((depth - dBlocker) / dBlocker) * (lightSize / frustum);

	//4 �������������pcfRadius
	return pcf(normal, lightDir, penumbra * pcfRadius);
}

void main()
{
//���������
	vec3 objectColor  = texture(sampler, uv).xyz ;
	vec3 result = vec3(0.0,0.0,0.0);

	//������յ�ͨ������
	vec3 normalN = normalize(normal);
	vec3 viewDir = normalize(worldPosition - cameraPosition);

	result += calculateDirectionalLight(objectColor, directionalLight,normalN, viewDir);

	
	float alpha =  texture(sampler, uv).a;

	vec3 ambientColor = objectColor * ambientColor;

	float shadow = pcss(lightSpacePosition, lightSpaceClipCoord, normal, -directionalLight.direction);
	vec3 finalColor = result * (1.0 - shadow) + ambientColor;

	FragColor = vec4(finalColor,alpha * opacity);
}