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