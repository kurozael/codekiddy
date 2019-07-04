/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CINPUTDEVICE_H
#define CINPUTDEVICE_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "Lua.h"

class CInputDevice
{
public:
	CInputDevice(CL_InputDevice& device);
	const char* GetName();
	const char* GetKeyName(int key);
	bool IsKeyDown(int key);
	CL_Vec2f GetPosition();
	int GetType();
	CL_InputDevice& Object();
	bool IsNull();
	static void LuaBind(luabind::object& globals);
	CInputDevice();
private:
	CL_InputDevice m_object;
};

#endif