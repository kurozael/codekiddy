/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef DISPLAY_H
#define DISPLAY_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "CDisplayWindow.h"
#include "CGraphicContext.h"
#include "CInputContext.h"
#include "CNonCopyable.h"
#include "CSingleton.h"
#include "Camera.h"
#include "Lua.h"

class Display : public CNonCopyable
{
public:
	~Display();
	int GetH();
	int GetW();
	CL_Mat4f GetViewMatrix();
	float ToFloat(const CL_Colorf& color);
	bool Create(unsigned int width, unsigned int height, const char* title);
	void SetSize(unsigned int width, unsigned int height, bool bRightNow);
	CL_Sizef GetDesktopSize();
	void SetWindowPos(int x, int y);
	void CenterWindow();
	CDisplayWindow& Window();
	CGraphicContext& Graphics();
	CInputContext& Inputs();
	void Clear(CL_Colorf& color);
	void Print(const char* text);
	void LuaBind(luabind::object& globals);
	void Update();
	void Flip(int interval = -1);
	Display();
protected:
	CGraphicContext m_graphics;
	CInputContext m_input;
	CDisplayWindow m_window;
	int m_newWidth;
	int m_newHeight;
};

#define g_Display CSingleton<Display>::Instance()

#endif