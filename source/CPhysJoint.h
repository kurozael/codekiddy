/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CPHYSJOINT_H
#define CPHYSJOINT_H

#include <ClanLib/core.h>
#include <Box2D/Box2D.h>
#include "CPhysBody.h"
#include "Lua.h"

class CPhysJoint
{
public:
	static void LuaBind(luabind::object& globals);
	CPhysBody GetBodyA();
	CPhysBody GetBodyB();
	CL_Vec2f GetAnchorA();
	CL_Vec2f GetAnchorB();
	void Destroy();
	b2Joint* Object();
	CPhysJoint(b2Joint* joint);
	CPhysJoint();
	~CPhysJoint();
protected:
	b2Joint* m_object;
};

#endif