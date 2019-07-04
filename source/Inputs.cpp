/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "Inputs.h"
#include "Display.h"

Inputs::Inputs()
{
	m_keyboard = CInputDevice( g_Display->Inputs().GetKeyboard() );
	m_mouse = CInputDevice( g_Display->Inputs().GetMouse() );
}

Inputs::~Inputs() {}

bool Inputs::IsKeyDown(int key)
{
	return m_keyboard.IsKeyDown(key);
}

const char* Inputs::GetKeyName(int key)
{
	return m_keyboard.GetKeyName(key);
}

CInputDevice& Inputs::Keyboard()
{
	return m_keyboard;
}

CInputDevice& Inputs::Mouse()
{
	return m_mouse;
}

CL_Vec2f Inputs::GetMousePos()
{
	return CL_Vec2f( m_mouse.GetPosition() );
}

void Inputs::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<Inputs>("Inputs")
		.def("GetKeyName", &Inputs::GetKeyName)
		.def("Keyboard", &Inputs::Keyboard)
		.def("IsKeyDown", &Inputs::IsKeyDown)
		.def("GetMousePos", &Inputs::GetMousePos)
		.def("Mouse", &Inputs::Mouse)
	];

	// Add the mouse enums to the global table.
	globals["MOUSE_LEFT"] = CL_MOUSE_LEFT;
	globals["MOUSE_RIGHT"] = CL_MOUSE_RIGHT;
	globals["MOUSE_MIDDLE"] = CL_MOUSE_MIDDLE;
	globals["MOUSE_WHEEL_UP"] = CL_MOUSE_WHEEL_UP;
	globals["MOUSE_WHEEL_DOWN"] = CL_MOUSE_WHEEL_DOWN;
	globals["MOUSE_XBUTTON1"] = CL_MOUSE_XBUTTON1;
	globals["MOUSE_XBUTTON2"] = CL_MOUSE_XBUTTON2;
	
	// Add the key enums to the global table.
	globals["KEY_BACKSPACE"] = CL_KEY_BACKSPACE;
	globals["KEY_TAB"] = CL_KEY_TAB;
	globals["KEY_CLEAR"] = CL_KEY_CLEAR;
	globals["KEY_RETURN"] = CL_KEY_RETURN;
	globals["KEY_SHIFT"] = CL_KEY_SHIFT;
	globals["KEY_CONTROL"] = CL_KEY_CONTROL;
	globals["KEY_MENU"] = CL_KEY_MENU;
	globals["KEY_PAUSE"] = CL_KEY_PAUSE;
	globals["KEY_ESCAPE"] = CL_KEY_ESCAPE;
	globals["KEY_SPACE"] = CL_KEY_SPACE;
	globals["KEY_PRIOR"] = CL_KEY_PRIOR;
	globals["KEY_NEXT"] = CL_KEY_NEXT;
	globals["KEY_END"] = CL_KEY_END;
	globals["KEY_HOME"] = CL_KEY_HOME;
	globals["KEY_LEFT"] = CL_KEY_LEFT;
	globals["KEY_UP"] = CL_KEY_UP;
	globals["KEY_RIGHT"] = CL_KEY_RIGHT;
	globals["KEY_DOWN"] = CL_KEY_DOWN;
	globals["KEY_SELECT"] = CL_KEY_SELECT;
	globals["KEY_PRINT"] = CL_KEY_PRINT;
	globals["KEY_EXECUTE"] = CL_KEY_EXECUTE;
	globals["KEY_INSERT"] = CL_KEY_INSERT;
	globals["KEY_DELETE"] = CL_KEY_DELETE;
	globals["KEY_HELP"] = CL_KEY_HELP;
	globals["KEY_0"] = CL_KEY_0;
	globals["KEY_1"] = CL_KEY_1;
	globals["KEY_2"] = CL_KEY_2;
	globals["KEY_3"] = CL_KEY_3;
	globals["KEY_4"] = CL_KEY_4;
	globals["KEY_5"] = CL_KEY_5;
	globals["KEY_6"] = CL_KEY_6;
	globals["KEY_7"] = CL_KEY_7;
	globals["KEY_8"] = CL_KEY_8;
	globals["KEY_9"] = CL_KEY_9;
	globals["KEY_A"] = CL_KEY_A;
	globals["KEY_B"] = CL_KEY_B;
	globals["KEY_C"] = CL_KEY_C;
	globals["KEY_D"] = CL_KEY_D;
	globals["KEY_E"] = CL_KEY_E;
	globals["KEY_F"] = CL_KEY_F;
	globals["KEY_G"] = CL_KEY_G;
	globals["KEY_H"] = CL_KEY_H;
	globals["KEY_I"] = CL_KEY_I;
	globals["KEY_J"] = CL_KEY_J;
	globals["KEY_K"] = CL_KEY_K;
	globals["KEY_L"] = CL_KEY_L;
	globals["KEY_M"] = CL_KEY_M;
	globals["KEY_N"] = CL_KEY_N;
	globals["KEY_O"] = CL_KEY_O;
	globals["KEY_P"] = CL_KEY_P;
	globals["KEY_Q"] = CL_KEY_Q;
	globals["KEY_R"] = CL_KEY_R;
	globals["KEY_S"] = CL_KEY_S;
	globals["KEY_T"] = CL_KEY_T;
	globals["KEY_U"] = CL_KEY_U;
	globals["KEY_V"] = CL_KEY_V;
	globals["KEY_W"] = CL_KEY_W;
	globals["KEY_X"] = CL_KEY_X;
	globals["KEY_Y"] = CL_KEY_Y;
	globals["KEY_Z"] = CL_KEY_Z;
	globals["KEY_LWIN"] = CL_KEY_LWIN;
	globals["KEY_RWIN"] = CL_KEY_RWIN;
	globals["KEY_APPS"] = CL_KEY_APPS;
	globals["KEY_NUMPAD0"] = CL_KEY_NUMPAD0;
	globals["KEY_NUMPAD1"] = CL_KEY_NUMPAD1;
	globals["KEY_NUMPAD2"] = CL_KEY_NUMPAD2;
	globals["KEY_NUMPAD3"] = CL_KEY_NUMPAD3;
	globals["KEY_NUMPAD4"] = CL_KEY_NUMPAD4;
	globals["KEY_NUMPAD5"] = CL_KEY_NUMPAD5;
	globals["KEY_NUMPAD6"] = CL_KEY_NUMPAD6;
	globals["KEY_NUMPAD7"] = CL_KEY_NUMPAD7;
	globals["KEY_NUMPAD8"] = CL_KEY_NUMPAD8;
	globals["KEY_NUMPAD9"] = CL_KEY_NUMPAD9;
	globals["KEY_NUMPAD_ENTER"] = CL_KEY_NUMPAD_ENTER;
	globals["KEY_MULTIPLY"] = CL_KEY_MULTIPLY;
	globals["KEY_ADD"] = CL_KEY_ADD;
	globals["KEY_SEPARATOR"] = CL_KEY_SEPARATOR;
	globals["KEY_SUBTRACT"] = CL_KEY_SUBTRACT;
	globals["KEY_DECIMAL"] = CL_KEY_DECIMAL;
	globals["KEY_DIVIDE"] = CL_KEY_DIVIDE;
	globals["KEY_F1"] = CL_KEY_F1;
	globals["KEY_F2"] = CL_KEY_F2;
	globals["KEY_F3"] = CL_KEY_F3;
	globals["KEY_F4"] = CL_KEY_F4;
	globals["KEY_F5"] = CL_KEY_F5;
	globals["KEY_F6"] = CL_KEY_F6;
	globals["KEY_F7"] = CL_KEY_F7;
	globals["KEY_F8"] = CL_KEY_F8;
	globals["KEY_F9"] = CL_KEY_F9;
	globals["KEY_F10"] = CL_KEY_F10;
	globals["KEY_F11"] = CL_KEY_F11;
	globals["KEY_F12"] = CL_KEY_F12;
	globals["KEY_F13"] = CL_KEY_F13;
	globals["KEY_F14"] = CL_KEY_F14;
	globals["KEY_F15"] = CL_KEY_F15;
	globals["KEY_F16"] = CL_KEY_F16;
	globals["KEY_F17"] = CL_KEY_F17;
	globals["KEY_F18"] = CL_KEY_F18;
	globals["KEY_F19"] = CL_KEY_F19;
	globals["KEY_F20"] = CL_KEY_F20;
	globals["KEY_F21"] = CL_KEY_F21;
	globals["KEY_F22"] = CL_KEY_F22;
	globals["KEY_F23"] = CL_KEY_F23;
	globals["KEY_F24"] = CL_KEY_F24;
	globals["KEY_NUMLOCK"] = CL_KEY_NUMLOCK;
	globals["KEY_SCROLL"] = CL_KEY_SCROLL;
	globals["KEY_LSHIFT"] = CL_KEY_LSHIFT;
	globals["KEY_RSHIFT"] = CL_KEY_RSHIFT;
	globals["KEY_LCONTROL"] = CL_KEY_LCONTROL;
	globals["KEY_RCONTROL"] = CL_KEY_RCONTROL;
	globals["KEY_LMENU"] = CL_KEY_LMENU;
	globals["KEY_RMENU"] = CL_KEY_RMENU;

	globals["InputsInstance"] = g_Inputs;
}