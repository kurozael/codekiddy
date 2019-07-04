/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef FONTS_H
#define FONTS_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "CNonCopyable.h"
#include "CFontDescription.h"
#include "CFontObject.h"
#include "CSingleton.h"
#include <string>
#include "Lua.h"
#include <map>

typedef std::map< std::string, std::pair<CFontDescription, CFontObject> > FontMap;

class Fonts : public CNonCopyable
{
public:
	~Fonts();
	Fonts();
	void LuaBind(luabind::object& globals);
	CFontDescription& AddFreetype(const char* fileName, const char* id, int height);
	CFontDescription& AddSystem(const char* fontName, const char* id, int height);
	void Update(const char* id);
	CFontObject GetFont(const char* id);
	CFontDescription& GetDesc(const char* id);
private:
	FontMap m_fontMap;
};

#define g_Fonts CSingleton<Fonts>::Instance()

#endif