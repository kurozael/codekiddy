/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "CPhysBody.h"
#include "CPhysData.h"
#include "Physics.h"

#define _USE_MATH_DEFINES
#include <math.h>

CPhysBody::CPhysBody(b2Body* body)
{
	m_object = body;
	m_bDestroyed = false;
}

CPhysBody::CPhysBody() : m_bDestroyed(false) {}

CPhysBody::~CPhysBody() {}

void CPhysBody::SetFixedRotation(bool fixedRotation)
{
	m_object->SetFixedRotation(fixedRotation);
}

bool CPhysBody::IsFixedRotation()
{
	return m_object->IsFixedRotation();
}

void CPhysBody::SetAngularVelocity(CL_Angle& angle)
{
	m_object->SetAngularVelocity( angle.to_degrees() );
}

CL_Angle CPhysBody::GetAngularVelocity()
{
	return CL_Angle(m_object->GetAngularVelocity(), cl_degrees);
}

void CPhysBody::SetVelocity(CL_Vec2f& velocity)
{
	m_object->SetLinearVelocity( g_Physics->SwapVec2(velocity) );
}

CL_Vec2f CPhysBody::GetVelocity()
{
	return g_Physics->SwapVec2( m_object->GetLinearVelocity() );
}

void CPhysBody::UpdateAsBox(float w, float h)
{
	b2Fixture* fixture = m_object->GetFixtureList();

	bool fixedRotation = m_object->IsFixedRotation();
	float friction = fixture->GetFriction();
	float density = fixture->GetDensity();
	bool isSensor = fixture->IsSensor();

	if (fixture && fixture != NULL)
		m_object->DestroyFixture(fixture);
	
	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(
		g_Physics->PixelsToMeters(w) / 2.0f, g_Physics->PixelsToMeters(h) / 2.0f,
		b2Vec2(0.0f, 0.0f), 0.0f
	);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &polygonShape;
	fixtureDef.friction = friction;
	fixtureDef.density = density;
	fixtureDef.isSensor = isSensor;

	m_object->CreateFixture(&fixtureDef);
	m_object->SetFixedRotation(fixedRotation);
}

void CPhysBody::UpdateAsCircle(float radius)
{
	b2Fixture* fixture = m_object->GetFixtureList();
	
	bool fixedRotation = m_object->IsFixedRotation();
	float friction = fixture->GetFriction();
	float density = fixture->GetDensity();
	bool isSensor = fixture->IsSensor();

	if (fixture && fixture != NULL)
		m_object->DestroyFixture(fixture);

	b2CircleShape circleShape;
	circleShape.m_radius = g_Physics->PixelsToMeters(radius) / 2;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.friction = friction;
	fixtureDef.density = density;
	fixtureDef.isSensor = isSensor;

	m_object->CreateFixture(&fixtureDef);
	m_object->SetFixedRotation(fixedRotation);
}

void CPhysBody::AddBody(CPhysBody& physBody)
{
	b2Fixture* fixture = physBody.Object()->GetFixtureList();

	while (fixture != NULL)
	{
		b2FixtureDef fixtureDef;
			fixtureDef.shape = fixture->GetShape();
			fixtureDef.friction = fixture->GetFriction();
			fixtureDef.density = fixture->GetDensity();
			fixtureDef.isSensor = fixture->IsSensor();
		m_object->CreateFixture(&fixtureDef);
		fixture = fixture->GetNext();
	}
}

void CPhysBody::SetPos(CL_Vec2f& position)
{
	m_object->SetTransform( g_Physics->SwapVec2(position), m_object->GetAngle() );
}

float Deg2Rad(float angle)
{
	return float(angle * (M_PI / 180));
}

void CPhysBody::SetAngle(CL_Angle& angle)
{
	//b2Vec2 vertexLocalPos = m_object->GetLocalCenter();
	//b2Vec2 vertexBeforeWorldPos = m_object->GetWorldPoint(vertexLocalPos);

	float curAngle = m_object->GetAngle();
	float totalRotation = angle.to_radians() - curAngle;

	while (totalRotation < Deg2Rad(-180)) totalRotation += Deg2Rad(360);
	while (totalRotation > Deg2Rad(180)) totalRotation -= Deg2Rad(360);

	m_object->SetTransform(m_object->GetPosition(), curAngle + totalRotation);

	//b2Vec2 vertexAfterWorldPos = m_object->GetWorldPoint(vertexLocalPos);
	//m_object->SetTransform(
	//	m_object->GetPosition() + vertexBeforeWorldPos - vertexAfterWorldPos, m_object->GetAngle()
	//);
}

CL_Angle CPhysBody::GetAngle()
{
	return CL_Angle(m_object->GetAngle(), cl_radians);
}

void CPhysBody::SetStatic(bool isStatic)
{
	if (isStatic)
		m_object->SetType(b2_staticBody);
	else
		m_object->SetType(b2_dynamicBody);
}

void CPhysBody::SetAngularDamping(float angularDamping)
{
	m_object->SetAngularDamping(angularDamping);
}

float CPhysBody::GetAngularDamping()
{
	return m_object->GetAngularDamping();
}

CL_Vec2f CPhysBody::GetLocalVector(CL_Vec2f& worldVector)
{
	return g_Physics->SwapVec2( m_object->GetLocalVector( g_Physics->SwapVec2(worldVector) ) );
}

CL_Vec2f CPhysBody::GetLocalCenter()
{
	return g_Physics->SwapVec2( m_object->GetLocalCenter() );
}

CL_Vec2f CPhysBody::GetWorldVector(CL_Vec2f& localVector)
{
	return g_Physics->SwapVec2( m_object->GetWorldVector( g_Physics->SwapVec2(localVector) ) );
}

CL_Vec2f CPhysBody::GetWorldCenter()
{
	return g_Physics->SwapVec2( m_object->GetWorldCenter() );
}

void CPhysBody::ApplyForce(CL_Vec2f& force, CL_Vec2f& origin)
{
	m_object->ApplyForce( g_Physics->SwapVec2(force), g_Physics->SwapVec2(origin) );
}

void CPhysBody::ApplyImpulse(CL_Vec2f& impulse, CL_Vec2f& origin)
{
	m_object->ApplyLinearImpulse( g_Physics->SwapVec2(impulse), g_Physics->SwapVec2(origin) );
}

void CPhysBody::ApplyTorque(float torque)
{
	m_object->ApplyTorque(torque);
}

void CPhysBody::SetDamping(float damping)
{
	m_object->SetLinearDamping(damping);
}

float CPhysBody::GetDamping()
{
	return m_object->GetLinearDamping();
}


void CPhysBody::SetFriction(float friction)
{
	b2Fixture* fixture = m_object->GetFixtureList();

	if (fixture && fixture != NULL)
		return fixture->SetFriction(friction);
}

float CPhysBody::GetFriction()
{
	b2Fixture* fixture = m_object->GetFixtureList();

	if (fixture && fixture != NULL)
		return fixture->GetFriction();

	return 0.0f;
}

void CPhysBody::SetDensity(float density)
{
	b2Fixture* fixture = m_object->GetFixtureList();

	if (fixture && fixture != NULL)
		return fixture->SetDensity(density);
}

float CPhysBody::GetDensity()
{
	b2Fixture* fixture = m_object->GetFixtureList();

	if (fixture && fixture != NULL)
		return fixture->GetDensity();

	return 0.0f;
}

void CPhysBody::SetSensor(bool isSensor)
{
	b2Fixture* fixture = m_object->GetFixtureList();

	if (fixture && fixture != NULL)
		fixture->SetSensor(isSensor);
}

bool CPhysBody::HitTest(CL_Vec2f& position)
{
	b2Fixture* fixture = m_object->GetFixtureList();
	
	if (fixture && fixture != NULL)
		return fixture->TestPoint( g_Physics->SwapVec2(position) );

	return false;
}

void CPhysBody::SetBullet(bool isBullet)
{
	m_object->SetBullet(isBullet);
}

bool CPhysBody::IsBullet()
{
	return m_object->IsBullet();
}


bool CPhysBody::IsSensor()
{
	b2Fixture* fixture = m_object->GetFixtureList();

	if (fixture && fixture != NULL)
		return fixture->IsSensor();
	else
		return false;
}

void CPhysBody::SetData(const char* data)
{
	CPhysData* userData = (CPhysData*)m_object->GetUserData();

	if (userData && userData != NULL)
		userData->SetValue(data);
}

std::string CPhysBody::GetData()
{
	CPhysData* userData = (CPhysData*)m_object->GetUserData();
	
	if (userData && userData != NULL)
		return userData->GetValue();
	else
		return "";
}

bool CPhysBody::IsAwake()
{
	return m_object->IsAwake();
}

void CPhysBody::WakeUp()
{
	m_object->SetAwake(true);
}

void CPhysBody::Sleep()
{
	m_object->SetAwake(false);
}

void CPhysBody::Destroy()
{
	if (!m_bDestroyed)
	{
		g_Physics->Destroy(*this);
		m_bDestroyed = true;
	}
}

bool CPhysBody::IsStatic()
{
	return (m_object->GetType() == b2_staticBody);
}

bool CPhysBody::IsValid()
{
	return !m_bDestroyed;
}

void CPhysBody::SetMass(float mass)
{
	b2MassData massData;
		m_object->GetMassData(&massData);
	massData.mass = mass;
	m_object->SetMassData(&massData);
}

float CPhysBody::GetMass()
{
	return m_object->GetMass();
}

CL_Vec2f CPhysBody::GetPos()
{
	return g_Physics->SwapVec2( m_object->GetPosition() );
}

b2Body* CPhysBody::Object()
{
	return m_object;
}

void CPhysBody::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<CPhysBody>("PhysBody")
		.def("SetAngularVelocity", &CPhysBody::SetAngularVelocity)
		.def("GetAngularVelocity", &CPhysBody::GetAngularVelocity)
		.def("IsFixedRotation", &CPhysBody::IsFixedRotation)
		.def("SetFixedRotation", &CPhysBody::SetFixedRotation)
		.def("SetVelocity", &CPhysBody::SetVelocity)
		.def("GetVelocity", &CPhysBody::GetVelocity)
		.def("UpdateAsBox", &CPhysBody::UpdateAsBox)
		.def("UpdateAsCircle", &CPhysBody::UpdateAsCircle)
		.def("SetStatic", &CPhysBody::SetStatic)
		.def("SetAngularDamping", &CPhysBody::SetAngularDamping)
		.def("GetAngularDamping", &CPhysBody::GetAngularDamping)
		.def("GetLocalVector", &CPhysBody::GetLocalVector)
		.def("GetLocalCenter", &CPhysBody::GetLocalCenter)
		.def("GetWorldVector", &CPhysBody::GetWorldVector)
		.def("GetWorldCenter", &CPhysBody::GetWorldCenter)
		.def("ApplyForce", &CPhysBody::ApplyForce)
		.def("ApplyImpulse", &CPhysBody::ApplyImpulse)
		.def("ApplyTorque", &CPhysBody::ApplyTorque)
		.def("SetDamping", &CPhysBody::SetDamping)
		.def("GetDamping", &CPhysBody::GetDamping)
		.def("SetFriction", &CPhysBody::SetFriction)
		.def("GetFriction", &CPhysBody::GetFriction)
		.def("SetDensity", &CPhysBody::SetDensity)
		.def("GetDensity", &CPhysBody::GetDensity)
		.def("SetSensor", &CPhysBody::SetSensor)
		.def("HitTest", &CPhysBody::HitTest)
		.def("SetBullet", &CPhysBody::SetBullet)
		.def("IsBullet", &CPhysBody::IsBullet)
		.def("IsSensor", &CPhysBody::IsSensor)
		.def("SetData", &CPhysBody::SetData)
		.def("GetData", &CPhysBody::GetData)
		.def("IsAwake", &CPhysBody::IsAwake)
		.def("WakeUp", &CPhysBody::WakeUp)
		.def("IsValid", &CPhysBody::IsValid)
		.def("Sleep", &CPhysBody::Sleep)
		.def("Destroy", &CPhysBody::Destroy)
		.def("IsStatic", &CPhysBody::IsStatic)
		.def("SetMass", &CPhysBody::SetMass)
		.def("GetMass", &CPhysBody::GetMass)
		.def("SetPos", &CPhysBody::SetPos)
		.def("SetAngle", &CPhysBody::SetAngle)
		.def("GetAngle", &CPhysBody::GetAngle)
		.def("AddBody", &CPhysBody::AddBody)
		.def("GetPos", &CPhysBody::GetPos)
	];
}