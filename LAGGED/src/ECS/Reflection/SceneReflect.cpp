#include "SceneReflect.h"

namespace LAG
{
	////////////////////////////////
	// COMPONENT REFLECTION SETUP //
	////////////////////////////////

	ComponentReflectionSetup::ComponentReflectionSetup(SceneReflect::ComponentData& compProperties) : 
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

	VariableReflectionSetup::VariableReflectionSetup(SceneReflect::VariableData& vars) :
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

	std::unordered_map<ComponentID, SceneReflect::ComponentData>& SceneReflect::GetComponentProperties()
	{
		return m_CompProperties;
	}


	/////////////////////////////
	// ITERATOR IMPLEMENTATION //
	/////////////////////////////

	SceneReflect::Iterator::Iterator(InnerIterator pIt, SceneReflect* pParent) : 
		m_pIt(pIt), m_pParent(pParent)
	{
	}

	SceneReflect::ComponentData* SceneReflect::Iterator::operator*() const
	{
		return &m_pIt->second;
	}

	SceneReflect::ComponentData& SceneReflect::Iterator::operator->() const
	{
		return m_pIt->second;
	}

	SceneReflect::Iterator& SceneReflect::Iterator::operator++()
	{
		m_pIt++;
		return *this;
	}

	SceneReflect::Iterator SceneReflect::Iterator::operator++(int)
	{
		Iterator tempIt = *this;
		++(*this);
		return tempIt;
	}

	SceneReflect::Iterator& SceneReflect::Iterator::operator--()
	{
		m_pIt--;
		return *this;
	}

	SceneReflect::Iterator SceneReflect::Iterator::operator--(int)
	{
		Iterator tempIt = *this;
		--(*this);
		return tempIt;
	}

	SceneReflect::Iterator SceneReflect::begin()
	{
		return Iterator(m_CompProperties.begin(), this);
	}

	SceneReflect::Iterator SceneReflect::end()
	{
		return Iterator(m_CompProperties.end(), this);
	}
}