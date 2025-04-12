#include "SceneReflect.h"

namespace LAG
{
	////////////////////////////////
	// COMPONENT REFLECTION SETUP //
	////////////////////////////////

	ComponentReflectionSetup::ComponentReflectionSetup(SceneReflect::ComponentClassProperties* compProperties)
	{
	}

	ComponentReflectionSetup& ComponentReflectionSetup::SetDisplayName(const std::string& displayName)
	{
		CRITICAL("TODO: Implement"); return *this;
	}
	ComponentReflectionSetup& ComponentReflectionSetup::SetHidden(bool isHidden)
	{
		CRITICAL("TODO: Implement"); return *this;
	}



	///////////////////////////////
	// VARIABLE REFLECTION SETUP //
	///////////////////////////////

	VariableReflectionSetup::VariableReflectionSetup(SceneReflect::ComponentVariableProperties* varProperties)
	{
	}

	VariableReflectionSetup& VariableReflectionSetup::SetDisplayName(const std::string& displayName)
	{
		CRITICAL("TODO: Implement"); return *this;
	}

	VariableReflectionSetup& VariableReflectionSetup::SetHidden(bool isHidden)
	{
		CRITICAL("TODO: Implement"); return *this;
	}

	VariableReflectionSetup& VariableReflectionSetup::SetReadOnly(bool isReadOnly)
	{
		CRITICAL("TODO: Implement"); return *this;
	}

}