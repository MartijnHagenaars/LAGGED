#version 330 core

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D a_Texture;

void main()
{
    vec3 col = texture(a_Texture, texCoord).rgb;
    FragColor = vec4(col, 1.0);
} 