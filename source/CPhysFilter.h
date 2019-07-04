/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CPHYSFILTER_H
#define CPHYSFILTER_H

#include <ClanLib/core.h>
#include <Box2D/Box2D.h>
#include "Lua.h"

class CPhysFilter : public b2ContactFilter
{
public:
	bool ShouldCollide(b2Fixture* first, b2Fixture* second);
	CPhysFilter();
	~CPhysFilter();
};

#endif