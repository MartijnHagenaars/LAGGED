#pragma once
#include "Utility/Hash.h"

namespace LAG
{
	class Window;
}

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
	
	//Add a new input action.
	//Returns true/false based on if the input action was added successfully or not. 
	bool AddInputAction(InputType inputType, StringHash64 actionName, const char* debugDisplayName = "InputAction");
	
	//Get an input action using it's ID.
	const InputActionData* GetInputAction(size_t inputID);

	//Returns true/false based on whether the action is pressed or not.
	bool IsActionPressed(StringHash64 actionName);

	//Returns true/false based on whether the action is pressed once. 
	//Will automatically turn false again when the button is held down for more than one frame.
	bool IsActionPressedOnce(StringHash64 actionName);

	//Contains two argument references that return the position of the cursor on the x- and y-axis.
	void GetMousePosition(float& xPos, float& yPos);

	//Get the input device type for an input type.
	//Determines if an input type is of input device Keyboard, Mouse or Controller.
	InputDeviceType GetInputDeviceType(const InputType& inputType);
}

