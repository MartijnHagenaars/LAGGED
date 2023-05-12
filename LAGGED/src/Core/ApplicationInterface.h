#pragma once
#include "Defines.h"

#define ENTRY_APP(x) LAG::IApplication* EntryPoint() { return new x; }

namespace LAG
{
	class IApplication
	{
	public:
		IApplication() {};
		virtual ~IApplication() { };

		virtual void Initialize() = 0;
		virtual void Shutdown() = 0;

		virtual void Update() = 0;

	};
}

LAG::IApplication* EntryPoint();
