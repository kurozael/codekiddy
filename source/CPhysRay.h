/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CPHYSRAY_H
#define CPHYSRAY_H

#include <ClanLib/core.h>
#include <Box2D/Box2D.h>
#include "CPhysBody.h"

struct RayData
{
	CL_Vec2f position;
	CPhysBody body;
	CL_Vec2f normal;
	float fraction;
	bool didHit;
};

class CPhysRay : public b2RayCastCallback
{
public:
	float ReportFixture(b2Fixture* fixture, const b2Vec2& position, const b2Vec2& normal, float fraction);
	RayData& GetData();
	bool DidHit();
	CPhysRay();
	~CPhysRay();
private:
	RayData m_data;
};

#endif