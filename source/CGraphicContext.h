/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CGraphicContext_H
#define CGraphicContext_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "Lua.h"

class CGraphicContext
{
public:
	CGraphicContext(CL_GraphicContext& graphics);
	CL_GraphicContext& Object();
	void PushBlendMode(CL_BlendEquation color, CL_BlendEquation alpha);
	void PopBlendMode();
	void PushRotation(CL_Angle& angle, CL_Vec2f& position);
	void PopRotation();
	void PushClipRect(CL_Rectf& rect);
	void PopClipRect();
	void Clear(CL_Colorf &color);
	void Reset();
	bool IsNull();
	int GetH();
	int GetW();
	static void LuaBind(luabind::object& globals);
	CGraphicContext();
private:
	CL_GraphicContext m_object;
	int m_rotationStack;
	int m_clipStack;
};

#endif