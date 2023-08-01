#pragma once
#include "Core/Input/Input.h"
#include "GLFW/glfw3.h"

int ConvertLAGInputToGLFWInput(LAG::Input::InputType inputType)
{
	switch (inputType)
	{
	case LAG::Input::InputType::LAG_LMB:			return GLFW_MOUSE_BUTTON_LEFT;
	case LAG::Input::InputType::LAG_RMB:			return GLFW_MOUSE_BUTTON_RIGHT;
	case LAG::Input::InputType::LAG_MMB:			return GLFW_MOUSE_BUTTON_MIDDLE;

	case LAG::Input::InputType::LAG_A:				return GLFW_KEY_A;
	case LAG::Input::InputType::LAG_B:				return GLFW_KEY_B;
	case LAG::Input::InputType::LAG_C:				return GLFW_KEY_C;
	case LAG::Input::InputType::LAG_D:				return GLFW_KEY_D;
	case LAG::Input::InputType::LAG_E:				return GLFW_KEY_E;
	case LAG::Input::InputType::LAG_F:				return GLFW_KEY_F;
	case LAG::Input::InputType::LAG_G:				return GLFW_KEY_G;
	case LAG::Input::InputType::LAG_H:				return GLFW_KEY_H;
	case LAG::Input::InputType::LAG_I:				return GLFW_KEY_I;
	case LAG::Input::InputType::LAG_J:				return GLFW_KEY_J;
	case LAG::Input::InputType::LAG_K:				return GLFW_KEY_K;
	case LAG::Input::InputType::LAG_L:				return GLFW_KEY_L;
	case LAG::Input::InputType::LAG_M:				return GLFW_KEY_M;
	case LAG::Input::InputType::LAG_N:				return GLFW_KEY_N;
	case LAG::Input::InputType::LAG_O:				return GLFW_KEY_O;
	case LAG::Input::InputType::LAG_P:				return GLFW_KEY_P;
	case LAG::Input::InputType::LAG_Q:				return GLFW_KEY_Q;
	case LAG::Input::InputType::LAG_R:				return GLFW_KEY_R;
	case LAG::Input::InputType::LAG_S:				return GLFW_KEY_S;
	case LAG::Input::InputType::LAG_T:				return GLFW_KEY_T;
	case LAG::Input::InputType::LAG_U:				return GLFW_KEY_U;
	case LAG::Input::InputType::LAG_V:				return GLFW_KEY_V;
	case LAG::Input::InputType::LAG_W:				return GLFW_KEY_W;
	case LAG::Input::InputType::LAG_X:				return GLFW_KEY_X;
	case LAG::Input::InputType::LAG_Y:				return GLFW_KEY_Y;
	case LAG::Input::InputType::LAG_Z:				return GLFW_KEY_Z;
	case LAG::Input::InputType::LAG_SPACE:			return GLFW_KEY_SPACE;
	case LAG::Input::InputType::LAG_SHIFT_L:		return GLFW_KEY_LEFT_SHIFT;
	case LAG::Input::InputType::LAG_SHIFT_R:		return GLFW_KEY_RIGHT_SHIFT;
	case LAG::Input::InputType::LAG_CONTROL_L:		return GLFW_KEY_LEFT_CONTROL;
	case LAG::Input::InputType::LAG_CONTROL_R:		return GLFW_KEY_RIGHT_CONTROL;
	case LAG::Input::InputType::LAG_ALT_L:			return GLFW_KEY_LEFT_ALT;
	case LAG::Input::InputType::LAG_ALT_R:			return GLFW_KEY_RIGHT_ALT;

	case LAG::Input::InputType::LAG_LEFT:			return GLFW_KEY_LEFT;
	case LAG::Input::InputType::LAG_UP:				return GLFW_KEY_UP;
	case LAG::Input::InputType::LAG_RIGHT:			return GLFW_KEY_RIGHT;
	case LAG::Input::InputType::LAG_DOWN:			return GLFW_KEY_DOWN;

	case LAG::Input::InputType::LAG_ENTER:			return GLFW_KEY_ENTER;
	case LAG::Input::InputType::LAG_BACK:			return GLFW_KEY_BACKSPACE;
	case LAG::Input::InputType::LAG_TAB:			return GLFW_KEY_TAB;

	case LAG::Input::InputType::LAG_NUM_0:			return GLFW_KEY_0;
	case LAG::Input::InputType::LAG_NUM_1:			return GLFW_KEY_1;
	case LAG::Input::InputType::LAG_NUM_2:			return GLFW_KEY_2;
	case LAG::Input::InputType::LAG_NUM_3:			return GLFW_KEY_3;
	case LAG::Input::InputType::LAG_NUM_4:			return GLFW_KEY_4;
	case LAG::Input::InputType::LAG_NUM_5:			return GLFW_KEY_5;
	case LAG::Input::InputType::LAG_NUM_6:			return GLFW_KEY_6;
	case LAG::Input::InputType::LAG_NUM_7:			return GLFW_KEY_7;
	case LAG::Input::InputType::LAG_NUM_8:			return GLFW_KEY_8;
	case LAG::Input::InputType::LAG_NUM_9:			return GLFW_KEY_9;

	case LAG::Input::InputType::LAG_ESCAPE:			return GLFW_KEY_ESCAPE;
	case LAG::Input::InputType::LAG_DELETE:			return GLFW_KEY_DELETE;
	case LAG::Input::InputType::LAG_CAPS:			return GLFW_KEY_CAPS_LOCK;
	case LAG::Input::InputType::LAG_PAUSE:			return GLFW_KEY_PAUSE;
	case LAG::Input::InputType::LAG_END:			return GLFW_KEY_END;
	case LAG::Input::InputType::LAG_HOME:			return GLFW_KEY_HOME;
	case LAG::Input::InputType::LAG_PRINTSCREEN:	return GLFW_KEY_PRINT_SCREEN;
	case LAG::Input::InputType::LAG_INSERT:			return GLFW_KEY_INSERT;

	default:case LAG::Input::InputType::LAG_UNKNOWN:return 0;
	}
}
