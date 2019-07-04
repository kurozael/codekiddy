/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "Fonts.h"
#include "Display.h"
#include "Util.h"

Fonts::Fonts() {}

Fonts::~Fonts() {}

CFontObject Fonts::GetFont(const char* id)
{
	return m_fontMap[ std::string(id) ].second;
}

CFontDescription& Fonts::GetDesc(const char* id)
{
	return m_fontMap[ std::string(id) ].first;
}

CFontDescription& Fonts::AddFreetype(const char* fontPath, const char* id, int height)
{
	std::string newFontPath = Util::FixFilePath(fontPath, "fonts/", ".ttf");
	std::string newID(id);

	m_fontMap[newID].first = CFontDescription();
	m_fontMap[newID].first.SetTypefaceName( newFontPath.c_str() );
	m_fontMap[newID].first.SetFontType(FONT_FREETYPE);
	m_fontMap[newID].first.SetAntiAlias(true);
	m_fontMap[newID].first.SetHeight(height);
	m_fontMap[newID].first.SetID(id);

	m_fontMap[newID].second = CFontObject(m_fontMap[newID].first);

	return m_fontMap[newID].first;
}

CFontDescription& Fonts::AddSystem(const char* fontName, const char* id, int height)
{
	std::string newID(id);

	m_fontMap[newID].first = CFontDescription();
	m_fontMap[newID].first.SetTypefaceName(fontName);
	m_fontMap[newID].first.SetFontType(FONT_SYSTEM);
	m_fontMap[newID].first.SetAntiAlias(true);
	m_fontMap[newID].first.SetHeight(height);
	m_fontMap[newID].first.SetID(id);

	m_fontMap[newID].second = CFontObject(m_fontMap[newID].first);

	return m_fontMap[newID].first;
}

void Fonts::Update(const char* id)
{
	std::string newID(id);
	m_fontMap[newID].second = CFontObject(m_fontMap[newID].first);
}

void Fonts::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<Fonts>("Fonts")
		.def("AddFreetype", &Fonts::AddFreetype)
		.def("AddSystem", &Fonts::AddSystem)
		.def("Update", &Fonts::Update)
		.def("GetFont", &Fonts::GetFont)
		.def("GetDesc", &Fonts::GetDesc)
	];

	globals["FontsInstance"] = g_Fonts;
}