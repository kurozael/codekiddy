/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "CVertexAttribute.h"

CVertexAttribute::CVertexAttribute(int usage, int numIdx, const char* alias) : usage(usage), numIdx(numIdx), offset(0), alias(alias) {}

void CVertexAttribute::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<CVertexAttribute>("VertexAttribute")
		.def(luabind::constructor<int, int, const char*>())
	];
}