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
			layout (location = 0) in vec3 aPos;
			layout (location = 1) in vec2 aTexCoord;

			uniform mat4 transformMat;
			out vec2 texCoord;
			
			void main()
			{
			    gl_Position = transformMat * vec4(aPos, 1.0);
				texCoord = vec2(aTexCoord.x, aTexCoord.y);
			}
		)";

		std::string pixel = R"(
			#version 330 core
			out vec4 ColorOutput;
			 
			in vec2 texCoord;
			uniform sampler2D testTexture;
			
			void main()
			{
			    ColorOutput = texture(testTexture, texCoord);
			} 
		)";
	} object;
}