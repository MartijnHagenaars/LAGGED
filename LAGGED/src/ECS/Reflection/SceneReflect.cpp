#include "SceneReflect.h"

namespace LAG
{
	////////////////////////////////
	// COMPONENT REFLECTION SETUP //
	////////////////////////////////

	ComponentReflectionSetup::ComponentReflectionSetup(ReflectionCompInfo& info) :
		m_Info(info)
	{
	}

	ComponentReflectionSetup& ComponentReflectionSetup::SetDisplayName(const std::string& displayName)
	{
		m_Info.props.displayName = displayName;
		return *this;
	}
	ComponentReflectionSetup& ComponentReflectionSetup::SetHidden(bool isHidden)
	{
		m_Info.props.isHidden = isHidden;
		return *this;
	}



	///////////////////////////////
	// VARIABLE REFLECTION SETUP //
	///////////////////////////////

	VariableReflectionSetup::VariableReflectionSetup(ReflectionCompInfo::MemberInfo& info) :
		m_Info(info)
	{
	}

	VariableReflectionSetup& VariableReflectionSetup::SetDisplayName(const std::string& displayName)
	{
		m_Info.props.displayName = displayName;
		return *this;
	}

	VariableReflectionSetup& VariableReflectionSetup::SetHidden(bool isHidden)
	{
		m_Info.props.isHidden = isHidden;
		return *this;
	}

	VariableReflectionSetup& VariableReflectionSetup::SetReadOnly(bool isReadOnly)
	{
		m_Info.props.isReadOnly = isReadOnly;
		return *this;
	}


	ReflectionCompInfo::Properties* SceneReflect::GetComponentProps(TypeID id)
	{
		if (const auto& compInfoIt = Scene::s_ReflectedCompInfo.find(id); compInfoIt != Scene::s_ReflectedCompInfo.end())
			return &compInfoIt->second.props;
		else 
			return nullptr;
	}
}