#pragma once
#include <wrl.h>
#include <dxgidebug.h>
#include <vector>
#include <string>

namespace LAG
{
	namespace InfoQueueManager
	{
		void Initialize();
		void Shutdown();

		//Inform the exception info queue when a new frame has started. 
		//This is important, as it will allow the developer to only receive exception info that is generated during that one single frame.
		void FrameStart();

		std::vector<std::string> GetExceptionInfoMessages();
	}
}
