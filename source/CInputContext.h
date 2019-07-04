/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CINPUTCONTEXT_H
#define CINPUTCONTEXT_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "CInputDevice.h"
#include "Lua.h"

class CInputContext
{
public:
	CInputContext(CL_InputContext& input);
	int GetKeyboardCount();
	int GetMouseCount();
	CInputDevice GetKeyboard();
	CInputDevice GetMouse();
	CL_InputContext& Object();
	static void LuaBind(luabind::object& globals);
	CInputContext();
private:
	CL_InputContext m_object;
};

#endif