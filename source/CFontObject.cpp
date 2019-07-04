/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "CFontObject.h"
#include "Display.h"
#include "Game.h"
#include "Files.h"

CFontObject::CFontObject() {}

CFontObject::CFontObject(CFontDescription &fontDesc)
{
	if (fontDesc.GetFontType() == FONT_FREETYPE)
	{
		if (g_Files->IsLocal(fontDesc.GetTypefaceName()))
			m_object = CL_Font_Freetype(fontDesc.Object());
		else
			m_object = CL_Font_Freetype(fontDesc.Object(), g_Game->GetVD());
	}else{
		m_object = CL_Font_System(g_Display->Graphics().Object(), fontDesc.Object());
	}
}

void CFontObject::DrawText(float x, float y, const char* text, const CL_Colorf& color)
{
	m_object.draw_text(g_Display->Graphics().Object(), int(x), int(y) + int(GetTextSize(text).height * 0.9f), text, color);
}

CL_Size CFontObject::GetTextSize(const char* text)
{
	CL_Size textSize = m_object.get_text_size(g_Display->Graphics().Object(), text);
		textSize.height = int(textSize.height * 0.8f);
	return textSize;
}

CL_Font& CFontObject::Object()
{
	return m_object;
}

bool CFontObject::IsNull()
{
	return m_object.is_null();
}

void CFontObject::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<CFontObject>("FontObject")
		.def(luabind::constructor<CFontDescription>())
		.def("GetTextSize", &CFontObject::GetTextSize)
		.def("DrawText", &CFontObject::DrawText)
		.def("IsNull", &CFontObject::IsNull)
	];
}