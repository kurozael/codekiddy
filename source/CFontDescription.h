/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CFONTDESCRIPTION_H
#define CFONTDESCRIPTION_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "Lua.h"

enum FontType
{
	FONT_FREETYPE,
	FONT_SYSTEM
};

class CFontDescription
{
public:
	const char* GetTypefaceName();
	void SetAntiAlias(bool aa);
	bool GetAntiAlias();
	void SetFontType(int fontType);
	int GetFontType() const;
	void SetTypefaceName(const char* name);
	void SetWeight(int weight);
	int GetWeight();
	void SetHeight(int height);
	CL_FontDescription& Object();
	int GetHeight();
	void SetID(const char* id);
	void Update();
	bool IsNull();
	static void LuaBind(luabind::object& globals);
	CFontDescription();
private:
	CL_FontDescription m_object;
	const char* m_id;
	int m_fontType;
};

#endif