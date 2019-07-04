/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "CPhysListener.h"
#include "CPhysBody.h"
#include "Physics.h"
#include "Lua.h"

CPhysListener::CPhysListener() {}

CPhysListener::~CPhysListener() {}

void CPhysListener::BeginContact(b2Contact* contact)
{
	b2Fixture* first = contact->GetFixtureA();
	b2Fixture* second = contact->GetFixtureB();

	CPhysBody firstBody( first->GetBody() );
	CPhysBody secondBody( second->GetBody() );

	b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);
	
	CL_Vec2f position = g_Physics->SwapVec2( worldManifold.points[0] );
	CL_Vec2f normal = g_Physics->SwapVec2(worldManifold.normal);

	//try
	//{

	luabind::call_function<void>(g_Lua->State(), "OnBodiesBeginContact", firstBody, secondBody, position, normal);
	
	//}
	//catch (const luabind::error &e)
	//{
	//	g_Lua->Print( lua_tostring(e.state(), -1) );
	//}
}

void CPhysListener::EndContact(b2Contact* contact)
{
	b2Fixture* first = contact->GetFixtureA();
	b2Fixture* second = contact->GetFixtureB();

	CPhysBody firstBody( first->GetBody() );
	CPhysBody secondBody( second->GetBody() );

	b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);
	
	CL_Vec2f position = g_Physics->SwapVec2( worldManifold.points[0] );
	CL_Vec2f normal = g_Physics->SwapVec2(worldManifold.normal);

	//try
	//{

	luabind::call_function<void>(g_Lua->State(), "OnBodiesEndContact", firstBody, secondBody, position, normal);
	
	//}
	//catch (const luabind::error &e)
	//{
	//	g_Lua->Print( lua_tostring(e.state(), -1) );
	//}
}

void CPhysListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	b2Fixture* first = contact->GetFixtureA();
	b2Fixture* second = contact->GetFixtureB();

	CPhysBody firstBody( first->GetBody() );
	CPhysBody secondBody( second->GetBody() );

	b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);
	
	CL_Vec2f position = g_Physics->SwapVec2( worldManifold.points[0] );
	CL_Vec2f normal = g_Physics->SwapVec2(worldManifold.normal);

	//try
	//{
	
	luabind::call_function<void>(g_Lua->State(), "OnPreSolveCollision", firstBody, secondBody, position, normal);
	
	//}
	//catch (const luabind::error &e)
	//{
	//	g_Lua->Print( lua_tostring(e.state(), -1) );
	//}
}

void CPhysListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	b2Fixture* first = contact->GetFixtureA();
	b2Fixture* second = contact->GetFixtureB();

	CPhysBody firstBody( first->GetBody() );
	CPhysBody secondBody( second->GetBody() );

	b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);
	
	CL_Vec2f position = g_Physics->SwapVec2( worldManifold.points[0] );
	CL_Vec2f normal = g_Physics->SwapVec2(worldManifold.normal);

	//try
	//{
	
	luabind::call_function<void>(g_Lua->State(), "OnPostSolveCollision", firstBody, secondBody, position, normal);
	
	//}
	//catch (const luabind::error &e)
	//{
	//	g_Lua->Print( lua_tostring(e.state(), -1) );
	//}
}