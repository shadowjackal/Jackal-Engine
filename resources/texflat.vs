#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 vnormals;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;
layout(location = 5) in ivec4 boneIds; 
layout(location = 6) in vec4 weights;

out vec3 ourColor;
out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 100;
uniform mat4 gBones[100];

void main()
{
	mat4 BoneTransform = gBones[boneIds[0]] * weights[0];
	BoneTransform += gBones[boneIds[1]] * weights[1];
	BoneTransform += gBones[boneIds[2]] * weights[2];
	BoneTransform += gBones[boneIds[3]] * weights[3];
	Normal = mat3(transpose(inverse(model))) * vnormals;

    vec4 PosL = BoneTransform * vec4(aPos, 1.0);

		 gl_Position = projection * view * model * PosL;
		 

	ourColor = vec3(0.5,0.5,0.5);
	FragPos = vec3(model * vec4(aPos, 1.0));
	TexCoords = vec2(aTexCoord.x, aTexCoord.y);
}