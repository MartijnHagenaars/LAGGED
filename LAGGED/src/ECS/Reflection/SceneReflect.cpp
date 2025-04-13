#include "SceneReflect.h"

namespace LAG
{
	////////////////////////////////
	// COMPONENT REFLECTION SETUP //
	////////////////////////////////

	ComponentReflectionSetup::ComponentReflectionSetup(SceneReflect::ComponentClassData& compProperties) : 
		m_Data(compProperties)
	{
	}

	ComponentReflectionSetup& ComponentReflectionSetup::SetDisplayName(const std::string& displayName)
	{
		m_Data.properties.displayName = displayName;
		return *this;
	}
	ComponentReflectionSetup& ComponentReflectionSetup::SetHidden(bool isHidden)
	{
		m_Data.properties.isHidden = isHidden;
		return *this;
	}



	///////////////////////////////
	// VARIABLE REFLECTION SETUP //
	///////////////////////////////

	VariableReflectionSetup::VariableReflectionSetup(SceneReflect::ComponentVariableData& vars) : 
		m_Data(vars)
	{
	}

	VariableReflectionSetup& VariableReflectionSetup::SetDisplayName(const std::string& displayName)
	{
		m_Data.properties.displayName = displayName;
		return *this;
	}

	VariableReflectionSetup& VariableReflectionSetup::SetHidden(bool isHidden)
	{
		m_Data.properties.isHidden = isHidden;
		return *this;
	}

	VariableReflectionSetup& VariableReflectionSetup::SetReadOnly(bool isReadOnly)
	{
		m_Data.properties.isReadOnly = isReadOnly;
		return *this;
	}
}