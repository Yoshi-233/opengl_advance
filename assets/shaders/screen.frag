#version 460 core
out vec4 FragColor;

in vec2 uv;

uniform sampler2D screenTexSampler;

uniform float texWidth;
uniform float texHeight;

//��ɫ����
vec3 colorInvert(vec3 color){
	vec3 invertColor = vec3(1.0) - color;
	return invertColor;
}

//ƽ���Ҷ�
vec3 gray(vec3 color){
	float avg = (color.r + color.g +color.b) / 3.0;
	return vec3(avg);
}

//�������۶�����ɫ�������У�����ɫ�����У�Ϊ��������ȷ
//����Ϊÿ����ɫͨ��������Ȩ��ֵ������ɫСһЩ����ɫ��һЩ
vec3 grayCorrect(vec3 color){
	float avg = color.r * 0.2126 + color.g * 0.7152 +color.b * 0.0722;
	return vec3(avg);
}


vec3 blur(){
	float du = 1.0 / texWidth;
	float dv = 1.0 / texHeight;

	//ƫ��ֵ����
	vec2 offsets[9]=vec2[](
		vec2(-du, dv),//����
		vec2(0.0, dv),//����
		vec2(du, dv),//����
		vec2(-du, 0.0),//��
		vec2(0.0, 0.0),//��
		vec2(du, 0.0),//��
		vec2(-du, -dv),//����
		vec2(0.0, -dv),//����
		vec2(du, -dv)//����
	);

	//������
	float kernel[9] = float[](
		1.0, 2.0, 1.0,
		2.0, 4.0, 2.0,
		1.0, 2.0, 1.0
	);

	//��Ȩ���
	vec3 sumColor = vec3(0.0);
	for(int i = 0;i < 9;i++){
		vec3 sampleColor = texture(screenTexSampler, uv + offsets[i]).rgb;
		sumColor += sampleColor * kernel[i];
	}
	sumColor /= 16.0;

	return sumColor;
}

void main()
{
	//vec3 color = colorInvert(texture(screenTexSampler, uv).rgb);
	//vec3 color = gray(texture(screenTexSampler, uv).rgb);
	//vec3 color = grayCorrect(texture(screenTexSampler, uv).rgb);
//	vec3 color = texture(screenTexSampler, uv).rgb;
//	vec3 color = blur();

	vec3 color = texture(screenTexSampler, uv).rgb;

	//2 ������ɫҪ�ֿ���Ļgamma
	color = pow(color, vec3(1.0/2.2));

	FragColor = vec4(color, 1.0);
}