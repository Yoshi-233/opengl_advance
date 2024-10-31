#version 460 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

uniform sampler2D sampler;	//diffuse��ͼ������
uniform sampler2D specularMaskSampler;//specularMask��ͼ������

uniform sampler2DArray shadowMapSampler;
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
	
	return max(bias * (1.0 - dot(normalN, lightDirN)), 0.0002);
}

uniform float pcfRadius;
float pcf(vec4 lightSpaceClipCoord, int layer, vec3 normal, vec3 lightDir,float pcfUVRadius){
	//1 �ҵ���ǰ�����ڹ�Դ�ռ��ڵ�NDC����
	vec3 lightNDC = lightSpaceClipCoord.xyz/lightSpaceClipCoord.w;

	//2 �ҵ���ǰ������ShadowMap�ϵ�uv
	vec3 projCoord = lightNDC * 0.5 + 0.5;
	vec2 uv = projCoord.xy;
	float depth = projCoord.z;

	poissonDiskSamples(uv);

	//3  ����poisson�����̵�ÿһ�������㣬ÿһ�������ֵ����Ҫ�뵱ǰ�����ڹ�Դ�µ����ֵ���жԱ�
	float sum = 0.0;
	for(int i = 0;i < NUM_SAMPLES;i++){
		float closestDepth = texture(shadowMapSampler,vec3(uv + disk[i] * pcfUVRadius,layer)).r;
		sum += closestDepth < (depth - getBias(normal, lightDir))?1.0:0.0;
	}

	return sum / float(NUM_SAMPLES);
}

uniform int csmLayerCount;
uniform float csmLayers[20];
uniform mat4 lightMatrices[20];
uniform mat4 viewMatrix;

int getCurrentLayer(vec3 positionWorldSpace){
	//��ǰ�������������ϵ�µ�����
	vec3 positionCameraSpace = (viewMatrix * vec4(positionWorldSpace, 1.0)).xyz;
	float z = -positionCameraSpace.z;

	int layer = 0;
	for(int i = 0;i <= csmLayerCount;i++){
		if(z < csmLayers[i]){
			layer = i - 1;
			break;
		}
	}

	return layer;
}

float csm(vec3 positionWorldSpace, vec3 normal, vec3 lightDir, float pcfRadius) {
	int layer = getCurrentLayer(positionWorldSpace);
	vec4 lightSpaceClipCoord = lightMatrices[layer] * vec4(positionWorldSpace, 1.0);

	return pcf(lightSpaceClipCoord,layer, normal, lightDir, pcfRadius);
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

	float shadow = csm(worldPosition, normal, -directionalLight.direction, pcfRadius);

	vec3 finalColor = result * (1.0 - shadow) + ambientColor;
	FragColor = vec4(finalColor,alpha * opacity);
}