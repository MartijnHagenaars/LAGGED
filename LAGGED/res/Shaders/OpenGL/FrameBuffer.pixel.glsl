#version 330 core

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D a_Texture;
uniform float a_InversionAmount;
uniform float a_GrayScaleAmount;

void main()
{
    //vec3 textureColor = texture(a_Texture, texCoord).rgb;

    vec4 textureColor = texture(a_Texture, texCoord);

    //Calculate inverted color
    textureColor.rgb = mix(textureColor.rgb, 1.0 - textureColor.rgb, clamp(a_InversionAmount, 0.0, 1.0));

    float averageColor = (textureColor.x + textureColor.y + textureColor.z) / 3.0;
    textureColor.rgb = mix(textureColor.rgb, vec3(averageColor, averageColor, averageColor) / 3.0, clamp(a_GrayScaleAmount, 0.0, 1.0));

    FragColor = vec4(textureColor.rgb, 1.0);
} 