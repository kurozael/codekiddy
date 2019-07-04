/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CVERTEXATTRIBUTE_H
#define CVERTEXATTRIBUTE_H

#include <string>
#include "Lua.h"

class CVertexAttribute
{
public:
	static void LuaBind(luabind::object& globals);
	CVertexAttribute(int usage, int numIdx, const char* alias);
	std::string alias;
	int numIdx;
	int usage;
	int offset;
};

#endif
