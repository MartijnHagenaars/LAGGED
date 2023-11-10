#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

out vec3 fragPosition;
out vec3 normal;
out vec2 texCoord;

uniform mat4 a_ModelMat;
uniform mat4 a_ViewMat;
uniform mat4 a_ProjMat;			

void main()
{
	fragPosition = vec3(a_ModelMat * vec4(a_Position, 1.f));
	normal = vec3(a_Normal.x, a_Normal.y, a_Normal.z);
	texCoord = vec2(a_TexCoord.x, a_TexCoord.y);

	normal = mat3(transpose(inverse(a_ModelMat))) * a_Normal;
	gl_Position = a_ProjMat * a_ViewMat * a_ModelMat * vec4(a_Position, 1.0);
}