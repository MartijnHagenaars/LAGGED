#pragma once
#include "Precomp.h"
#include "ApplicationInterface.h"

#include "Engine.h"

extern LAG::IApplication* EntryPoint();

int main()
{
	return LAG::GetEngine().Run(EntryPoint());
}