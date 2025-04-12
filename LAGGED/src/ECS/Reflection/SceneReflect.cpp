#include "SceneReflect.h"

namespace LAG
{
	////////////////////////////////
	// COMPONENT REFLECTION SETUP //
	////////////////////////////////

	ComponentReflectionSetup::ComponentReflectionSetup(SceneReflect::ComponentClassProperties& compProperties) : 
		m_Props(compProperties)
	{
	}

	ComponentReflectionSetup& ComponentReflectionSetup::SetDisplayName(const std::string& displayName)
	{
		m_Props.displayName = displayName;
		return *this;
	}
	ComponentReflectionSetup& ComponentReflectionSetup::SetHidden(bool isHidden)
	{
		m_Props.isHidden = isHidden;
		return *this;
	}



	///////////////////////////////
	// VARIABLE REFLECTION SETUP //
	///////////////////////////////

	VariableReflectionSetup::VariableReflectionSetup(SceneReflect::ComponentVariableProperties& varProperties) : 
		m_Props(varProperties)
	{
	}

	VariableReflectionSetup& VariableReflectionSetup::SetDisplayName(const std::string& displayName)
	{
		m_Props.displayName = displayName;
		CRITICAL("TODO: Implement"); return *this;
	}

	VariableReflectionSetup& VariableReflectionSetup::SetHidden(bool isHidden)
	{
		m_Props.isHidden = isHidden;
		return *this;
	}

	VariableReflectionSetup& VariableReflectionSetup::SetReadOnly(bool isReadOnly)
	{
		m_Props.isReadOnly = isReadOnly;
		return *this;
	}

}