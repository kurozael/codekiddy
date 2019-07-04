/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef INPUTS_H
#define INPUT_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "CInputDevice.h"
#include "CNonCopyable.h"
#include "CSingleton.h"
#include "Lua.h"

class Inputs : public CNonCopyable
{
public:
	bool IsKeyDown(int key);
	const char* GetKeyName(int key);
	CInputDevice& Keyboard();
	CInputDevice& Mouse();
	CL_Vec2f GetMousePos();
	void LuaBind(luabind::object& globals);
	~Inputs();
	Inputs();
private:
	CInputDevice m_keyboard;
	CInputDevice m_mouse;
};

#define g_Inputs CSingleton<Inputs>::Instance()

#endif