#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

out vec2 uv;
out vec3 normal;
out vec3 worldPosition;
out mat3 tbn;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform mat3 normalMatrix;

//aPos��Ϊattribute�����ԣ�����shader
//��������ĵ�
void main()
{
// ������Ķ���λ�ã�ת��Ϊ������꣨3ά-4ά��
	vec4 transformPosition = vec4(aPos, 1.0);

	//��һ���м����TransformPosition�����ڼ�����λλ����modelMatrix��˵��м���
	transformPosition = modelMatrix * transformPosition;

	//���㵱ǰ�����worldPosition������������FragmentShader
	worldPosition = transformPosition.xyz;

	gl_Position = projectionMatrix * viewMatrix * transformPosition;
	
	uv = aUV;
//	normal =  transpose(inverse(mat3(modelMatrix))) * aNormal;
	normal =  normalMatrix * aNormal;
	vec3 tangent = normalize(mat3(modelMatrix) * aTangent);
	vec3 bitangent = normalize(cross(normal, tangent));
	tbn = mat3(tangent, bitangent, normal);
}