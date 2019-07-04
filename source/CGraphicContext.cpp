/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "CGraphicContext.h"
#include "Camera.h"

CGraphicContext::CGraphicContext()
{
	m_rotationStack = 0;
	m_clipStack = 0;
}

CGraphicContext::CGraphicContext(CL_GraphicContext& graphics)
{
	m_rotationStack = 0;
	m_clipStack = 0;
	m_object = graphics;
}

CL_GraphicContext& CGraphicContext::Object()
{
	return m_object;
}

int CGraphicContext::GetH()
{
	return m_object.get_height();
}

int CGraphicContext::GetW()
{
	return m_object.get_height();
}

void CGraphicContext::Clear(CL_Colorf &color)
{
	m_object.clear(color);
	m_object.clear_depth();
	m_object.clear_stencil();
}

void CGraphicContext::Reset()
{
	m_object.reset_cliprect();
	m_object.reset_blend_mode();
	m_object.reset_program_object();

	m_clipStack = 0;
	m_rotationStack = 0;

	g_Camera->ClearStack();
}

bool CGraphicContext::IsNull()
{
	return m_object.is_null();
}

void CGraphicContext::PushClipRect(CL_Rectf& rect)
{
	CL_Vec2f position(rect.left, rect.top);
	CL_Vec2f size(rect.right, rect.bottom);

	CL_Rect window = CL_Rect(
		(int)(position.x + 0.5f), (int)(position.y + 0.5f),
		(int)(size.x + 0.5f), (int)(size.y + 0.5f)
	);

	m_object.push_cliprect(window);
	m_clipStack++;
}

void CGraphicContext::PopClipRect()
{
	if (m_clipStack > 0)
	{
		m_object.pop_cliprect();
		m_clipStack--;
	}
}

void CGraphicContext::PushRotation(CL_Angle& angle, CL_Vec2f& position)
{
	CL_Vec2f cameraPos = g_Camera->GetPos();
	float x = floor(position.x);
	float y = floor(position.y);

	m_object.push_modelview();
		m_object.mult_translate(x, y);
			m_object.mult_rotate(angle);
		m_object.mult_translate(-x, -y);
	m_rotationStack++;
}

void CGraphicContext::PopRotation()
{
	if (m_rotationStack > 0)
	{
		m_object.pop_modelview();
		m_rotationStack--;
	}
}

void CGraphicContext::PushBlendMode(CL_BlendEquation color, CL_BlendEquation alpha)
{
	CL_BlendMode blender;
		blender.enable_blending(true);
		blender.set_blend_equation(color, alpha);
	m_object.set_blend_mode(blender);
}

void CGraphicContext::PopBlendMode()
{
	m_object.reset_blend_mode();
}

void CGraphicContext::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<CGraphicContext>("GraphicContext")
		.def("PushBlendMode", &CGraphicContext::PushBlendMode)
		.def("PopBlendMode", &CGraphicContext::PopBlendMode)
		.def("PushClipRect", &CGraphicContext::PushClipRect)
		.def("PopClipRect", &CGraphicContext::PopClipRect)
		.def("PushRotation", &CGraphicContext::PushRotation)
		.def("PopRotation", &CGraphicContext::PopRotation)
		.def("IsNull", &CGraphicContext::IsNull)
		.def("GetW", &CGraphicContext::GetW)
		.def("GetH", &CGraphicContext::GetH)
	];
}