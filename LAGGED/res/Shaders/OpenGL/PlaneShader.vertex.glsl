#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

out vec3 fragPosition;

uniform mat4 a_ModelMat;
uniform mat4 a_ViewMat;
uniform mat4 a_ProjMat;		

void main()
{
	gl_Position = a_ProjMat * a_ViewMat * a_ModelMat * vec4(a_Position, 1.0);
}