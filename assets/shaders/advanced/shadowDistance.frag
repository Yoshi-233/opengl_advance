#version 460 core

in vec3 worldPosition;

uniform float far;
uniform vec3 lightPosition;

//����Ҫ����κ���ɫ
//���ڻ���ShadowMap��FBOֻ����ȸ���û����ɫ����
void main(){
	float lightDis = length(worldPosition - lightPosition);
	lightDis = lightDis / (1.414 * far);

	gl_FragDepth = lightDis;
}