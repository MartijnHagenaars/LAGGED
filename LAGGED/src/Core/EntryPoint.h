#pragma once
#include "Precomp.h"
#include "ApplicationInterface.h"

#include "Engine.h"
//#include "Platform/PlatformBase/WindowBase.h"

extern LAG::IApplication* EntryPoint();

int main()
{
	return LAG::Engine().Run(EntryPoint());
}