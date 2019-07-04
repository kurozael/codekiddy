/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "CPhysFilter.h"
#include "CPhysBody.h"
#include "Lua.h"

CPhysFilter::CPhysFilter() {}

CPhysFilter::~CPhysFilter() {}

bool CPhysFilter::ShouldCollide(b2Fixture* first, b2Fixture* second)
{
	CPhysBody firstBody = CPhysBody( first->GetBody() );
	CPhysBody secondBody = CPhysBody( second->GetBody() );

	//try
	//{

	bool shouldCollide = luabind::call_function<bool>(g_Lua->State(), "OnBodiesShouldCollide", firstBody, secondBody);
	return shouldCollide;

	//}
	//catch (const luabind::error &e)
	//{
	//	g_Lua->Print( lua_tostring(e.state(), -1) );
	//}
}