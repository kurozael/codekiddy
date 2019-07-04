/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CPHYSBODY_H
#define CPHYSBODY_H

#include <ClanLib/core.h>
#include <Box2D/Box2D.h>
#include <string>
#include "Lua.h"

class CPhysBody
{
public:
	static void LuaBind(luabind::object& globals);
	void SetAngularVelocity(CL_Angle& angle);
	CL_Angle GetAngularVelocity();
	void SetFixedRotation(bool fixedRotation);
	bool IsFixedRotation();
	void SetVelocity(CL_Vec2f& velocity);
	CL_Vec2f GetVelocity();
	void UpdateAsBox(float w, float h);
	void UpdateAsCircle(float radius);
	void AddBody(CPhysBody& physBody);
	void SetPos(CL_Vec2f& position);
	void SetAngle(CL_Angle& angle);
	void SetStatic(bool isStatic);
	void SetAngularDamping(float angularDamping);
	float GetAngularDamping();
	CL_Vec2f GetLocalVector(CL_Vec2f& worldVector);
	CL_Vec2f GetLocalCenter();
	CL_Vec2f GetWorldVector(CL_Vec2f& localVector);
	CL_Vec2f GetWorldCenter();
	void ApplyForce(CL_Vec2f& force, CL_Vec2f& origin);
	void ApplyImpulse(CL_Vec2f& impulse, CL_Vec2f& origin);
	void ApplyTorque(float torque);
	void SetDamping(float damping);
	float GetDamping();
	void SetFriction(float friction);
	float GetFriction();
	void SetDensity(float density);
	float GetDensity();
	void SetSensor(bool isSensor);
	bool HitTest(CL_Vec2f& position);
	void SetBullet(bool isBullet);
	bool IsBullet();
	bool IsSensor();
	void SetData(const char* data);
	std::string GetData();
	bool IsAwake();
	bool IsValid();
	void WakeUp();
	void Sleep();
	void Destroy();
	bool IsStatic();
	void SetMass(float mass);
	float GetMass();
	CL_Angle GetAngle();
	CL_Vec2f GetPos();
	b2Body* Object();
	CPhysBody(b2Body* body);
	CPhysBody();
	~CPhysBody();
protected:
	b2Body* m_object;
	bool m_bDestroyed;
};

#endif