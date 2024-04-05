#version 330 core

layout(location = 0) in vec3 a_Position;
uniform mat4 a_MVP;

void main() {
	gl_Position = a_MVP * vec4(a_Position, 1.0);
}