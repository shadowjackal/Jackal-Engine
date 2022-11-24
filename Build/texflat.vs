#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 vnormals;
layout (location = 3) in vec2 aTexCoord;


out vec3 ourColor;
out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	Normal = mat3(transpose(inverse(model))) * vnormals;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	ourColor = aColor;
	FragPos = vec3(model * vec4(aPos, 1.0));
	TexCoords = vec2(aTexCoord.x, aTexCoord.y);
}