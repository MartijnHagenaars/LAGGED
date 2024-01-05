#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
//layout (location = 2) in vec2 a_TexCoord;

//TODO: Check if I really need to out the frag position here
out vec3 fragPosition;
out vec3 normal;

uniform mat4 a_ModelMat;
uniform mat4 a_ViewMat;
uniform mat4 a_ProjMat;		

void main()
{
	fragPosition = a_Position;
	normal = a_Normal;
	gl_Position = a_ProjMat * a_ViewMat * a_ModelMat * vec4(a_Position, 1.0);
}