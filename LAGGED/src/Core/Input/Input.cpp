#include "Precomp.h"
#include "Input.h"

#include <unordered_map>

namespace LAG::Input
{
	//These structs are causing errors. Fix!!
	std::unordered_map<size_t, InputActionData> inputActions;


	bool AddInputAction(InputType inputType, Utility::String actionName, const char* debugDisplayName)
	{
		//size_t hashedID = std::hash<std::string>{}(actionName);
		if (inputActions.find(actionName.GetValue()) == inputActions.end())
		{
			Utility::Logger::Warning("Input action with ID \"{0}\" already exists.", actionName.GetValue());
			return false;
		}

#ifndef DEBUG
		InputActionData actionData = { inputType, actionName.GetValue() };
#else
		InputActionData actionData = { inputType, actionName.GetValue(), debugDisplayName };
#endif
		inputActions.emplace(actionName.GetValue(), actionData);

		return true;
	}

	bool IsActionPressed(Utility::String actionName)
	{
		auto it = inputActions.find(actionName.GetValue());
		if (it == inputActions.end())
		{
			Utility::Logger::Error("Input action with ID \"{0}\" not found.", actionName.GetValue());
			return false;
		}
		else
		{
			//TODO: Add implementation for checking input here
			it->second.type;
			return true;
		}
	}

	InputDeviceType GetInputDeviceType(InputType& inputType)
	{
		if (inputType >= InputType::LAG_LMB && inputType <= InputType::LAG_MMB)
			return InputDeviceType::LAG_MOUSE;
		else if (inputType >= InputType::LAG_A && inputType <= InputType::LAG_INSERT)
			return InputDeviceType::LAG_KEYBOARD;
		//No controller support yet...
		else return InputDeviceType::LAG_UNKNOWN;
	}
}

