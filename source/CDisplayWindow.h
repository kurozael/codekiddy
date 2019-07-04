/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CDISPLAYWINDOW_H
#define CDISPLAYWINDOW_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "Lua.h"

class CDisplayWindow
{
public:
	CDisplayWindow(CL_DisplayWindow& window);
	CL_DisplayWindow& Object();
	void Flip(int interval = -1);
	CL_Size GetSize();
	void Minimize();
	void Restore();
	bool IsVisible();
	bool IsMinimized();
	bool IsMaximized();
	bool IsFullScreen();
	void ShowCursor();
	void HideCursor();
	void SetCursor(const char* fileName, CL_Origin origin);
	void SetCursor(int type);
	void SetTitle(const char* title);
	const char* GetTitle();
	void SetIcon(const char* fileName);
	bool IsNull();
	static void LuaBind(luabind::object& globals);
	CDisplayWindow();
private:
	CL_DisplayWindow m_object;
};

#endif