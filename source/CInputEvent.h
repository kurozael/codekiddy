/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CINPUTEVENT_H
#define CINPUTEVENT_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "CInputDevice.h"
#include "Lua.h"

class CInputEvent
{
public:
	CInputEvent(CL_InputEvent& input);
	CL_Vec2f GetMousePos();
	CInputDevice& GetInputDevice();
	int GetID();
	int GetType();
	const char* GetString();
	bool IsAlt();
	bool IsShift();
	bool IsCtrl();
	CL_InputEvent& Object();
	static void LuaBind(luabind::object& globals);
	CInputEvent();
private:
	CInputDevice m_inputDevice;
	CL_InputEvent m_object;
};

#endif