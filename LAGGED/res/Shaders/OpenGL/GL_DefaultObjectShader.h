#pragma once
#include "GL_ShaderBase.h"

namespace LAG::ShaderData
{
	struct Object : public GLShaderBase
	{
		const std::string& GetVertex() const override { return vertex; }
		const std::string& GetPixel() const override { return pixel; }
	private:
		std::string vertex = R"(
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
		)";

		std::string pixel = R"(
			#version 330 core

			in vec3 fragPosition;
			in vec3 normal;
			in vec2 texCoord;

			out vec4 colorOut;

			uniform sampler2D a_Texture1;

			//Lighting data
			uniform vec3 a_LightPosition;
			uniform vec3 a_LightColor;
			uniform float a_LightIntensity;
			uniform float a_LightAttenuation;
			
			vec3 CalculateDiffuse()
			{
				vec3 lightDirection = normalize(a_LightPosition - fragPosition);
				float diffuseIntensity = max(0.0, dot(normal, lightDirection));
				return diffuseIntensity * a_LightColor;
			}

			float CalculateAttenuation()
			{
				float lightDistance = distance(fragPosition, a_LightPosition);
				return a_LightIntensity * (1.0 / (1.0 + lightDistance + a_LightAttenuation * lightDistance * lightDistance));
			}

			void main()
			{
				vec3 lightCalculation = (CalculateDiffuse()) * CalculateAttenuation();
			    colorOut = texture(a_Texture1, texCoord) * vec4(lightCalculation, 1.f);
			    //colorOut = vec4(normal, 1.f); //Draw normal
			} 
		)";
	} object;
}