#version 330 core

layout (location = 0) in vec3 a_Position;

out vec3 texCoord;

uniform mat4 a_ViewMat;
uniform mat4 a_ProjMat;

void main()
{
	texCoord = a_Position;
	gl_Position = (a_ProjMat * a_ViewMat * vec4(a_Position, 1.0)).xyww;
}