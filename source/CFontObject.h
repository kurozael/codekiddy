/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CFONTOBJECT_H
#define CFONTOBJECT_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "CFontDescription.h"
#include "Lua.h"

class CFontObject
{
public:
	CFontObject(CFontDescription &fontDesc);
	void DrawText(float x, float y, const char* text, const CL_Colorf& color);
	CL_Font& Object();
	CL_Size GetTextSize(const char* text);
	bool IsNull();
	static void LuaBind(luabind::object& globals);
	CFontObject();
private:
	CL_Font m_object;
};

#endif