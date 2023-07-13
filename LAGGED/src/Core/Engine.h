#pragma once
#include <memory>
#include "Defines.h"

#include "Core/ApplicationInterface.h"
#include "Platform/Base/WindowBase.h"
#include "Platform/Base/RendererBase.h"

namespace LAG
{
	class Engine
	{
	public:
		LAG_API Engine();
		LAG_API ~Engine();

		LAG_API int Run(IApplication* applicationPtr);

	private:
		bool Initialize(IApplication* applicationPtr);
		bool Shutdown();

		void EventCallback(EventBase& event);

	private:
		std::unique_ptr<LAG::IApplication> m_App;
	};
}
