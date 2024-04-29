#version 330 core

out vec4 colorOut;

uniform vec3 a_LineColor;

void main() {
	colorOut = vec4(a_LineColor, 1.0);
}