/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "CPhysRay.h"
#include "CPhysBody.h"
#include "Physics.h"
#include "Lua.h"

CPhysRay::CPhysRay()
{
	m_data.fraction = 1.0f;
	m_data.didHit = false;
}

CPhysRay::~CPhysRay() {}

float CPhysRay::ReportFixture(b2Fixture* fixture, const b2Vec2& position, const b2Vec2& normal, float fraction)
{
	CPhysBody body( fixture->GetBody() );

	//try
	//{

	bool canHit = luabind::call_function<bool>(g_Lua->State(), "OnShouldRayHitBody", body);
		
	if (!canHit)
		return -1;

	//}
	//catch (const luabind::error &e)
	//{
	//	g_Lua->Print( lua_tostring(e.state(), -1) );
	//}
	
	if (fraction <= m_data.fraction)
	{
		m_data.position = g_Physics->SwapVec2(position);
		m_data.fraction = fraction;
		m_data.didHit = true;
		m_data.normal = g_Physics->SwapVec2(normal);
		m_data.body = body;
	}
	
	return fraction;
}

bool CPhysRay::DidHit()
{
	return m_data.didHit;
}

RayData& CPhysRay::GetData()
{
	return m_data;
}