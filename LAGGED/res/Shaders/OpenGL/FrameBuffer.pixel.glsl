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
    vec3 invertedColor = 1.0 - textureColor.rgb;

    //Calculate grayscale
    vec3 grayScaleColor = textureColor.rgb / 3.0;

    vec3 inversionMix = mix(textureColor.rgb, invertedColor, clamp(a_InversionAmount, 0.0, 1.0));
    //vec3 grayScaleMix = mix(invertedColor, grayScaleColor, clamp(a_GrayScaleAmount, 0.0, 1.0));
    FragColor = vec4(inversionMix, 1.0);
} 