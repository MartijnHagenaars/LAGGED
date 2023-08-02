#pragma once
#include "Utility/String.h"

namespace LAG::Input
{
	enum class InputType : unsigned char
	{
		//Mouse input types
		LAG_LMB = 0,
		LAG_RMB,
		LAG_MMB,


		//Keyboard input types
		LAG_A,
		LAG_B,
		LAG_C,
		LAG_D,
		LAG_E,
		LAG_F,
		LAG_G,
		LAG_H,
		LAG_I,
		LAG_J,
		LAG_K,
		LAG_L,
		LAG_M,
		LAG_N,
		LAG_O,
		LAG_P,
		LAG_Q,
		LAG_R,
		LAG_S,
		LAG_T,
		LAG_U,
		LAG_V,
		LAG_W,
		LAG_X,
		LAG_Y,
		LAG_Z,
		LAG_SPACE,
		LAG_SHIFT_L,
		LAG_SHIFT_R,
		LAG_CONTROL_L,
		LAG_CONTROL_R,
		LAG_ALT_L,
		LAG_ALT_R,

		LAG_LEFT,
		LAG_UP,
		LAG_RIGHT,
		LAG_DOWN,

		LAG_ENTER,
		LAG_BACK,
		LAG_TAB,

		LAG_NUM_0,
		LAG_NUM_1,
		LAG_NUM_2,
		LAG_NUM_3,
		LAG_NUM_4,
		LAG_NUM_5,
		LAG_NUM_6,
		LAG_NUM_7,
		LAG_NUM_8,
		LAG_NUM_9,

		LAG_ESCAPE,
		LAG_DELETE,
		LAG_CAPS,
		LAG_PAUSE,
		LAG_END,
		LAG_HOME,
		LAG_PRINTSCREEN,
		LAG_INSERT,

		LAG_UNKNOWN
	};

	enum class InputDeviceType
	{
		LAG_MOUSE = 0,
		LAG_KEYBOARD,
		LAG_CONTROLLER,
		LAG_UNKNOWN
	};

	struct InputActionData
	{
		InputActionData(InputType& type, size_t ID, const char* actionName = "Undef") :
			type(type), actionID(ID), actionName(actionName)
		{ }

		//Other important variables
		InputType type = InputType::LAG_UNKNOWN;
		size_t actionID = 0;

		const char* actionName = "Undef";
		const char* GetDebugDisplayName() const { return actionName; }
	};

	LAG_API bool AddInputAction(InputType inputType, Utility::String actionName, const char* debugDisplayName = "InputAction");
	const InputActionData* GetInputAction(size_t inputID);

	LAG_API bool IsActionPressed(Utility::String actionName);
	LAG_API bool IsActionPressedOnce(Utility::String actionName);

	//Contains two argument references that return the position of the cursor on the x- and y-axis.
	LAG_API void GetMousePosition(float& xPos, float& yPos);

	InputDeviceType GetInputDeviceType(const InputType& inputType);
}

