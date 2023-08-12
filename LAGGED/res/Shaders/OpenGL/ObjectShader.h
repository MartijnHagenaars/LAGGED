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
			  
			out vec4 vertexColor;
			
			void main()
			{
			    gl_Position = vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
			    vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // set the output variable to a dark-red color
			}
		)";

		std::string pixel = R"(
			#version 330 core
			out vec4 FragColor;
			  
			in vec4 vertexColor;
			
			void main()
			{
			    FragColor = vertexColor;
			} 
		)";
	} object;
}