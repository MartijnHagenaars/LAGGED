#include "SceneReflect.h"

namespace LAG
{
	////////////////////////////////
	// COMPONENT REFLECTION SETUP //
	////////////////////////////////

	ComponentReflectionSetup::ComponentReflectionSetup(SceneReflect::ComponentReflectionData& compProperties) : 
		m_Data(compProperties)
	{
	}

	ComponentReflectionSetup& ComponentReflectionSetup::SetDisplayName(const std::string& displayName)
	{
		m_Data.props.displayName = displayName;
		return *this;
	}
	ComponentReflectionSetup& ComponentReflectionSetup::SetHidden(bool isHidden)
	{
		m_Data.props.isHidden = isHidden;
		return *this;
	}



	///////////////////////////////
	// VARIABLE REFLECTION SETUP //
	///////////////////////////////

	VariableReflectionSetup::VariableReflectionSetup(SceneReflect::VariableReflectionData& vars) :
		m_Data(vars)
	{
	}

	VariableReflectionSetup& VariableReflectionSetup::SetDisplayName(const std::string& displayName)
	{
		m_Data.props.displayName = displayName;
		return *this;
	}

	VariableReflectionSetup& VariableReflectionSetup::SetHidden(bool isHidden)
	{
		m_Data.props.isHidden = isHidden;
		return *this;
	}

	VariableReflectionSetup& VariableReflectionSetup::SetReadOnly(bool isReadOnly)
	{
		m_Data.props.isReadOnly = isReadOnly;
		return *this;
	}

	SceneReflect& SceneReflect::Get()
	{
		static SceneReflect sceneReflect;
		return sceneReflect;
	}

	std::unordered_map<ComponentID, SceneReflect::ComponentReflectionData>& SceneReflect::GetComponentProperties()
	{
		return m_CompProperties;
	}
}