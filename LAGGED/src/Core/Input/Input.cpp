#include "Precomp.h"
#include "Input.h"

#include <unordered_map>
#include "Platform/Base/Window/WindowBase.h"
#include "Platform/Base/Window/WindowManager.h"

namespace LAG::Input
{
	//These structs are causing errors. Fix!!
	std::unordered_map<size_t, InputActionData> inputActions;


	bool AddInputAction(InputType inputType, Utility::String actionName, const char* debugDisplayName)
	{
		//Check if input action already exists
		if (inputActions.find(actionName.GetValue()) != inputActions.end())
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

	bool IsInputActionValid(size_t actionValue, std::unordered_map<size_t, LAG::Input::InputActionData>::iterator& iteratorOut)
	{
		iteratorOut = inputActions.find(actionValue);
		if (iteratorOut == inputActions.end())
		{
			Utility::Logger::Error("Input action with ID \"{0}\" not found.", actionValue);
			return false;
		}
		else return true;
	}

	const InputActionData* GetInputAction(size_t inputID)
	{
		std::unordered_map<size_t, LAG::Input::InputActionData>::iterator it;
		if (IsInputActionValid(inputID, it))
			return &it->second;
		else return nullptr;
	}

	bool IsActionPressed(Utility::String actionName)
	{
		return IsActionPressed(actionName, WindowManager::Get().GetFocussedWindow());
	}

	bool IsActionPressed(Utility::String actionName, LAG::WindowBase* window)
	{
		std::unordered_map<size_t, LAG::Input::InputActionData>::iterator it;
		if (window != nullptr && IsInputActionValid(actionName.GetValue(), it))
			return window->CheckButtonPress(it->second, false);
		else return false;
	}

	bool IsActionPressedOnce(Utility::String actionName)
	{
		return IsActionPressedOnce(actionName, WindowManager::Get().GetFocussedWindow());
	}

	bool IsActionPressedOnce(Utility::String actionName, LAG::WindowBase* window)
	{
		std::unordered_map<size_t, LAG::Input::InputActionData>::iterator it;
		if (window != nullptr && IsInputActionValid(actionName.GetValue(), it))
			return window->CheckButtonPress(it->second, true);
		else return false;
	}

	void GetMousePosition(float& xPos, float& yPos)
	{
		GetMousePosition(xPos, yPos, WindowManager::Get().GetFocussedWindow());
	}

	void GetMousePosition(float& xPos, float& yPos, LAG::WindowBase* window)
	{
		window->GetMousePosition(xPos, yPos);
	}

	InputDeviceType GetInputDeviceType(const InputType& inputType)
	{
		if (inputType >= InputType::LAG_LMB && inputType <= InputType::LAG_MMB)
			return InputDeviceType::LAG_MOUSE;
		else if (inputType >= InputType::LAG_A && inputType <= InputType::LAG_INSERT)
			return InputDeviceType::LAG_KEYBOARD;
		//No controller support yet...
		else return InputDeviceType::LAG_UNKNOWN;
	}
}

