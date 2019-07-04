/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef PHYSICS_H
#define PHYSICS_H

#include <ClanLib/core.h>
#include "CNonCopyable.h"
#include <Box2D/Box2D.h>
#include "CPhysListener.h"
#include "CPhysFilter.h"
#include "CSingleton.h"
#include "CPhysJoint.h"
#include "CPhysBody.h"
#include "Lua.h"

class Physics : public CNonCopyable
{
public:
	void LuaBind(luabind::object& globals);
	CPhysJoint DistanceJoint(CPhysBody& bodyA, CPhysBody& bodyB, CL_Vec2f& anchorA, CL_Vec2f& anchorB, bool bCollideConnected);
	CPhysBody CreateCircle(float radius);
	CPhysBody CreateBox(float w, float h);
	void RayCast(CL_Vec2f& startPos, CL_Vec2f& endPos);
	void DebugDraw(CPhysBody& body, CL_Colorf& color);
	void SetPixelsToMeters(int pixelsToMeters);
	int GetPixelsToMeters();
	float MetersToPixels(float meters);
	float PixelsToMeters(float pixels);
	CL_Vec2f SwapVec2(const b2Vec2& vector);
	b2Vec2 SwapVec2(CL_Vec2f& vector);
	void SetGravity(CL_Vec2f& gravity);
	CPhysBody FromPNG(const char* fileName);
	CL_Vec2f GetGravity();
	CPhysJoint WeldJoint(CPhysBody& bodyA, CPhysBody& bodyB, CL_Vec2f& anchor, bool bCollideConnected);
	void SetIterations(int position, int velocity);
	void Destroy(CPhysBody& body);
	void Destroy(CPhysJoint& joint);
	void Update(float deltaTime);
	void Init();
	~Physics();
	Physics();
private:
	CPhysListener m_listener;
	CPhysFilter m_filter;
	b2World* m_world;
	int m_pixelsToMeters;
	int m_posIter;
	int m_velIter;
};

#define g_Physics CSingleton<Physics>::Instance()

#endif