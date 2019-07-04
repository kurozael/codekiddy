/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "CFontDescription.h"
#include "Fonts.h"

CFontDescription::CFontDescription()
{
	m_fontType = FONT_SYSTEM;
}

const char* CFontDescription::GetTypefaceName()
{
	return m_object.get_typeface_name().c_str();
}

void CFontDescription::SetTypefaceName(const char* name)
{
	m_object.set_typeface_name(name);
	m_object.set_subpixel(false);
}

CL_FontDescription& CFontDescription::Object()
{
	return m_object;
}

void CFontDescription::SetFontType(int fontType)
{
	m_fontType = fontType;
}

int CFontDescription::GetFontType() const
{
	return m_fontType;
}

void CFontDescription::SetAntiAlias(bool aa)
{
	m_object.set_anti_alias(aa);
}

bool CFontDescription::GetAntiAlias()
{
	return m_object.get_anti_alias();
}

void CFontDescription::SetWeight(int weight)
{
	m_object.set_weight(weight);
}

int CFontDescription::GetWeight()
{
	return m_object.get_weight();
}

void CFontDescription::SetHeight(int height)
{
	m_object.set_height(height);
}

int CFontDescription::GetHeight()
{
	return m_object.get_height();
}

void CFontDescription::Update()
{
	g_Fonts->Update(m_id);
}

bool CFontDescription::IsNull()
{
	return m_object.is_null();
}

void CFontDescription::SetID(const char* id)
{
	m_id = id;
}

void CFontDescription::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<CFontDescription>("FontDescription")
		.def(luabind::constructor<>())
		.def("SetTypefaceName", &CFontDescription::SetTypefaceName)
		.def("GetTypefaceName", &CFontDescription::GetTypefaceName)
		.def("SetFontType", &CFontDescription::SetFontType)
		.def("GetFontType", &CFontDescription::GetFontType)
		.def("SetAntiAlias", &CFontDescription::SetAntiAlias)
		.def("GetAntiAlias", &CFontDescription::GetAntiAlias)
		.def("SetWeight", &CFontDescription::SetWeight)
		.def("GetWeight", &CFontDescription::GetWeight)
		.def("SetHeight", &CFontDescription::SetHeight)
		.def("GetHeight", &CFontDescription::GetHeight)
		.def("Update", &CFontDescription::Update)
		.def("IsNull", &CFontDescription::IsNull)
	];

	globals["FONT_FREETYPE"] = FONT_FREETYPE;
	globals["FONT_SYSTEM"] = FONT_SYSTEM;
}