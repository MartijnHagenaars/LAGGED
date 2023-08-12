#pragma once
#include "ShaderDataBase.h"

namespace LAG::ShaderData
{
	struct Object : public ShaderDataBase
	{
		const std::string& GetVertex() const override { return vertex; }
		const std::string& GetPixel() const override { return pixel; }
	private:
		std::string vertex = R"(
			#version 330 core
			layout (location = 0) in vec3 aPos;
			layout (location = 1) in vec2 aTexCoord;
			  
			out vec4 vertexColor;
			out vec2 texCoord;
			
			void main()
			{
			    gl_Position = vec4(aPos, 1.0);
			    vertexColor = vec4(0.5, 0.0, 0.0, 1.0);
				texCoord = aTexCoord;
			}
		)";

		std::string pixel = R"(
			#version 330 core
			out vec4 ColorOutput;
			  
			in vec4 vertexColor;
			in vec2 texCoord;
			uniform sampler2D testTexture;
			
			void main()
			{
				//ColorOutput = vec4(0.5, 0.1, 0.1, 1.0);
			    ColorOutput = texture(testTexture, texCoord);
			} 
		)";
	} object;
}