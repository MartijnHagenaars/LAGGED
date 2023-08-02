#pragma once
#include "Core/Input/Input.h"

//Virtual key code values taken from here: https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
LAG::Input::InputType ConvertWIN32InputToLAGInput(int win32Input)
{
	switch (win32Input)
	{
	case 0x01:			return LAG::Input::InputType::LAG_LMB;
	case 0x02:			return LAG::Input::InputType::LAG_RMB;
	case 0x04:			return LAG::Input::InputType::LAG_MMB;			

	case 0x41:			return LAG::Input::InputType::LAG_A;			
	case 0x42:			return LAG::Input::InputType::LAG_B;			
	case 0x43:			return LAG::Input::InputType::LAG_C;			
	case 0x44:			return LAG::Input::InputType::LAG_D;			
	case 0x45:			return LAG::Input::InputType::LAG_E;			
	case 0x46:			return LAG::Input::InputType::LAG_F;			
	case 0x47:			return LAG::Input::InputType::LAG_G;			
	case 0x48:			return LAG::Input::InputType::LAG_H;			
	case 0x49:			return LAG::Input::InputType::LAG_I;			
	case 0x4A:			return LAG::Input::InputType::LAG_J;			
	case 0x4B:			return LAG::Input::InputType::LAG_K;			
	case 0x4C:			return LAG::Input::InputType::LAG_L;			
	case 0x4D:			return LAG::Input::InputType::LAG_M;			
	case 0x4E:			return LAG::Input::InputType::LAG_N;			
	case 0x4F:			return LAG::Input::InputType::LAG_O;			
	case 0x50:			return LAG::Input::InputType::LAG_P;			
	case 0x51:			return LAG::Input::InputType::LAG_Q;			
	case 0x52:			return LAG::Input::InputType::LAG_R;			
	case 0x53:			return LAG::Input::InputType::LAG_S;			
	case 0x54:			return LAG::Input::InputType::LAG_T;			
	case 0x55:			return LAG::Input::InputType::LAG_U;			
	case 0x56:			return LAG::Input::InputType::LAG_V;			
	case 0x57:			return LAG::Input::InputType::LAG_W;			
	case 0x58:			return LAG::Input::InputType::LAG_X;			
	case 0x59:			return LAG::Input::InputType::LAG_Y;			
	case 0x5A:			return LAG::Input::InputType::LAG_Z;			
	case 0x20:			return LAG::Input::InputType::LAG_SPACE;
	case 0xA0:			return LAG::Input::InputType::LAG_SHIFT_L;
	case 0xA1:			return LAG::Input::InputType::LAG_SHIFT_R;
	case 0xA2:			return LAG::Input::InputType::LAG_CONTROL_L;
	case 0xA3:			return LAG::Input::InputType::LAG_CONTROL_R;
	case 0xA4:			return LAG::Input::InputType::LAG_ALT_L;
	case 0xA5:			return LAG::Input::InputType::LAG_ALT_R;

	case 0x25:			return LAG::Input::InputType::LAG_LEFT;
	case 0x26:			return LAG::Input::InputType::LAG_UP;
	case 0x27:			return LAG::Input::InputType::LAG_RIGHT;
	case 0x28:			return LAG::Input::InputType::LAG_DOWN;

	case 0x0D:			return LAG::Input::InputType::LAG_ENTER;
	case 0x08:			return LAG::Input::InputType::LAG_BACK;
	case 0x09:			return LAG::Input::InputType::LAG_TAB;

	case 0x60:			return LAG::Input::InputType::LAG_NUM_0;
	case 0x61:			return LAG::Input::InputType::LAG_NUM_1;		
	case 0x62:			return LAG::Input::InputType::LAG_NUM_2;		
	case 0x63:			return LAG::Input::InputType::LAG_NUM_3;		
	case 0x64:			return LAG::Input::InputType::LAG_NUM_4;		
	case 0x65:			return LAG::Input::InputType::LAG_NUM_5;		
	case 0x66:			return LAG::Input::InputType::LAG_NUM_6;		
	case 0x67:			return LAG::Input::InputType::LAG_NUM_7;		
	case 0x68:			return LAG::Input::InputType::LAG_NUM_8;		
	case 0x69:			return LAG::Input::InputType::LAG_NUM_9;		

	case 0x1B:			return LAG::Input::InputType::LAG_ESCAPE;		
	case 0x2E:			return LAG::Input::InputType::LAG_DELETE;		
	case 0x14:			return LAG::Input::InputType::LAG_CAPS;
	case 0x13:			return LAG::Input::InputType::LAG_PAUSE;
	case 0x23:			return LAG::Input::InputType::LAG_END;
	case 0x24:			return LAG::Input::InputType::LAG_HOME;
	case 0x2C:			return LAG::Input::InputType::LAG_PRINTSCREEN;
	case 0x2D:			return LAG::Input::InputType::LAG_INSERT;

	default:case 0: return LAG::Input::InputType::LAG_UNKNOWN;
	}
}

unsigned char ConvertLAGInputToWIN32(LAG::Input::InputType inputType)
{
	switch (inputType)
	{
	case LAG::Input::InputType::LAG_LMB:			return 0x01;
	case LAG::Input::InputType::LAG_RMB:			return 0x02;
	case LAG::Input::InputType::LAG_MMB:			return 0x04;

	case LAG::Input::InputType::LAG_A:				return 0x41;
	case LAG::Input::InputType::LAG_B:				return 0x42;
	case LAG::Input::InputType::LAG_C:				return 0x43;
	case LAG::Input::InputType::LAG_D:				return 0x44;
	case LAG::Input::InputType::LAG_E:				return 0x45;
	case LAG::Input::InputType::LAG_F:				return 0x46;
	case LAG::Input::InputType::LAG_G:				return 0x47;
	case LAG::Input::InputType::LAG_H:				return 0x48;
	case LAG::Input::InputType::LAG_I:				return 0x49;
	case LAG::Input::InputType::LAG_J:				return 0x4A;
	case LAG::Input::InputType::LAG_K:				return 0x4B;
	case LAG::Input::InputType::LAG_L:				return 0x4C;
	case LAG::Input::InputType::LAG_M:				return 0x4D;
	case LAG::Input::InputType::LAG_N:				return 0x4E;
	case LAG::Input::InputType::LAG_O:				return 0x4F;
	case LAG::Input::InputType::LAG_P:				return 0x50;
	case LAG::Input::InputType::LAG_Q:				return 0x51;
	case LAG::Input::InputType::LAG_R:				return 0x52;
	case LAG::Input::InputType::LAG_S:				return 0x53;
	case LAG::Input::InputType::LAG_T:				return 0x54;
	case LAG::Input::InputType::LAG_U:				return 0x55;
	case LAG::Input::InputType::LAG_V:				return 0x56;
	case LAG::Input::InputType::LAG_W:				return 0x57;
	case LAG::Input::InputType::LAG_X:				return 0x58;
	case LAG::Input::InputType::LAG_Y:				return 0x59;
	case LAG::Input::InputType::LAG_Z:				return 0x5A;
	case LAG::Input::InputType::LAG_SPACE:			return 0x20;
	case LAG::Input::InputType::LAG_SHIFT_L:		return 0xA0;
	case LAG::Input::InputType::LAG_SHIFT_R:		return 0xA1;
	case LAG::Input::InputType::LAG_CONTROL_L:		return 0xA2;
	case LAG::Input::InputType::LAG_CONTROL_R:		return 0xA3;
	case LAG::Input::InputType::LAG_ALT_L:			return 0xA4;
	case LAG::Input::InputType::LAG_ALT_R:			return 0xA5;

	case LAG::Input::InputType::LAG_LEFT:			return 0x25;
	case LAG::Input::InputType::LAG_UP:				return 0x26;
	case LAG::Input::InputType::LAG_RIGHT:			return 0x27;
	case LAG::Input::InputType::LAG_DOWN:			return 0x28;

	case LAG::Input::InputType::LAG_ENTER:			return 0x0D;
	case LAG::Input::InputType::LAG_BACK:			return 0x08;
	case LAG::Input::InputType::LAG_TAB:			return 0x09;

	case LAG::Input::InputType::LAG_NUM_0:			return 0x60;
	case LAG::Input::InputType::LAG_NUM_1:			return 0x61;
	case LAG::Input::InputType::LAG_NUM_2:			return 0x62;
	case LAG::Input::InputType::LAG_NUM_3:			return 0x63;
	case LAG::Input::InputType::LAG_NUM_4:			return 0x64;
	case LAG::Input::InputType::LAG_NUM_5:			return 0x65;
	case LAG::Input::InputType::LAG_NUM_6:			return 0x66;
	case LAG::Input::InputType::LAG_NUM_7:			return 0x67;
	case LAG::Input::InputType::LAG_NUM_8:			return 0x68;
	case LAG::Input::InputType::LAG_NUM_9:			return 0x69;

	case LAG::Input::InputType::LAG_ESCAPE:			return 0x1B;
	case LAG::Input::InputType::LAG_DELETE:			return 0x2E;
	case LAG::Input::InputType::LAG_CAPS:			return 0x14;
	case LAG::Input::InputType::LAG_PAUSE:			return 0x13;
	case LAG::Input::InputType::LAG_END:			return 0x23;
	case LAG::Input::InputType::LAG_HOME:			return 0x24;
	case LAG::Input::InputType::LAG_PRINTSCREEN:	return 0x2C;
	case LAG::Input::InputType::LAG_INSERT:			return 0x2D;

	default:case LAG::Input::InputType::LAG_UNKNOWN:return 0;
	}
}