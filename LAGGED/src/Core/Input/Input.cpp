#include "Precomp.h"
#include "Input.h"

#include <unordered_map>
#include "Core/Engine.h"
#include "Platform/Base/Window/WindowBase.h"
#include "Platform/Base/Window/WindowManager.h"

namespace LAG::Input
{
	std::unordered_map<size_t, InputActionData> inputActions;


	bool AddInputAction(InputType inputType, HashedString actionName, const char* debugDisplayName)
	{
		//Check if input action already exists
		if (inputActions.find(actionName.GetValue()) != inputActions.end())
		{
			Logger::Warning("Input action with ID \"{0}\" already exists.", actionName.GetValue());
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
			Logger::Error("Input action with ID \"{0}\" not found.", actionValue);
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

	bool IsActionPressed(HashedString actionName)
	{
		return IsActionPressed(actionName, GetWindowManager()->GetFocussedWindow());
	}

	bool IsActionPressed(HashedString actionName, LAG::WindowBase* window)
	{
		std::unordered_map<size_t, LAG::Input::InputActionData>::iterator it;
		if (window != nullptr && IsInputActionValid(actionName.GetValue(), it))
			return window->CheckButtonPress(it->second, false);
		else return false;
	}

	bool IsActionPressedOnce(HashedString actionName)
	{
		return IsActionPressedOnce(actionName, GetWindowManager()->GetFocussedWindow());
	}

	bool IsActionPressedOnce(HashedString actionName, LAG::WindowBase* window)
	{
		std::unordered_map<size_t, LAG::Input::InputActionData>::iterator it;
		if (window != nullptr && IsInputActionValid(actionName.GetValue(), it))
			return window->CheckButtonPress(it->second, true);
		else return false;
	}

	void GetMousePosition(float& xPos, float& yPos)
	{
		GetMousePosition(xPos, yPos, GetWindowManager()->GetFocussedWindow());
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

