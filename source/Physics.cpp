/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "Physics.h"
#include "Display.h"
#include "CPhysData.h"
#include "CPhysRay.h"
#include "CMarchingSquare.h"
#include "Render.h"

Physics::Physics()
{
	m_world = new b2World(b2Vec2(0.0f, 0.0f), true);
	m_posIter = 3;
	m_velIter = 8;
	m_pixelsToMeters = 64;
}

Physics::~Physics()
{
	delete m_world;
	m_world = NULL;
}

CPhysJoint Physics::DistanceJoint(CPhysBody& bodyA, CPhysBody& bodyB, CL_Vec2f& anchorA, CL_Vec2f& anchorB, bool bCollideConnected)
{
	b2DistanceJointDef jointDef;
		jointDef.Initialize(
				bodyA.Object(), bodyB.Object(),
				SwapVec2(anchorA), SwapVec2(anchorB)
			);
	jointDef.collideConnected = bCollideConnected;

	return CPhysJoint( m_world->CreateJoint(&jointDef) ); 
}

void Physics::SetPixelsToMeters(int pixelsToMeters)
{
	m_pixelsToMeters = pixelsToMeters;
}

int Physics::GetPixelsToMeters()
{
	return m_pixelsToMeters;
}

CPhysBody Physics::CreateCircle(float radius)
{
	b2BodyDef bodyDef;
	bodyDef.angularDamping = 4.5f;
	bodyDef.linearDamping = 5.0f;
	bodyDef.userData = new CPhysData();
	bodyDef.active = true;
	bodyDef.type = b2_dynamicBody;

	b2CircleShape circleShape;
	circleShape.m_radius = PixelsToMeters(radius) / 2;

	b2FixtureDef fixtureDef;
	fixtureDef.friction = 0.5f;
	fixtureDef.density = 0.1f;
	fixtureDef.shape = &circleShape;

	b2Body* body = m_world->CreateBody(&bodyDef);
	b2Fixture* fixture = body->CreateFixture(&fixtureDef);
	body->SetFixedRotation(false);

	return CPhysBody(body);
}

CPhysBody Physics::CreateBox(float w, float h)
{
	b2BodyDef bodyDef;
	bodyDef.angularDamping = 4.5f;
	bodyDef.linearDamping = 5.0f;
	bodyDef.userData = new CPhysData();
	bodyDef.active = true;
	bodyDef.type = b2_dynamicBody;

	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(
		PixelsToMeters(w) / 2.0f, PixelsToMeters(h) / 2.0f, b2Vec2(0.0f, 0.0f), 0.0f
	);

	b2FixtureDef fixtureDef;
	fixtureDef.friction = 0.5f;
	fixtureDef.density = 0.1f;
	fixtureDef.shape = &polygonShape;
	
	b2Body* body = m_world->CreateBody(&bodyDef);
	b2Fixture* fixture = body->CreateFixture(&fixtureDef);
	body->SetFixedRotation(false);

	return CPhysBody(body);
}

void Physics::DebugDraw(CPhysBody& body, CL_Colorf& color)
{
	b2Body* bodyPtr = body.Object();
	const b2Transform& transform = bodyPtr->GetTransform();
	const b2Vec2& bodyPos = bodyPtr->GetPosition();
	b2Fixture* fixture = bodyPtr->GetFixtureList();
	
	g_Display->Graphics().Object().set_rotate(CL_Angle(transform.GetAngle(), CL_AngleUnit::cl_radians), bodyPos.x, bodyPos.y);

	while (fixture != NULL)
	{
		b2PolygonShape* shape = (b2PolygonShape*)fixture->GetShape();
		b2Vec2 firstPos(0, 0);
		b2Vec2 lastPos(0, 0);
		
		for (int i = 0; i < shape->GetVertexCount(); i++)
		{
			const b2Vec2& vertex = shape->GetVertex(i);
			
			if (i == 0) {
				lastPos.x = bodyPos.x + vertex.x; lastPos.y = bodyPos.y + vertex.y;
				firstPos = lastPos;
				continue;
			};

			g_Render->DrawLine(
				lastPos.x, lastPos.y, bodyPos.x + vertex.x, bodyPos.y + vertex.y, color
			);

			lastPos.x = bodyPos.x + vertex.x;
			lastPos.y = bodyPos.y + vertex.y;
		}

		g_Render->DrawLine(
			lastPos.x, lastPos.y, firstPos.x, firstPos.y, color
		);
		
		fixture = fixture->GetNext();
	}

	g_Display->Graphics().Object().set_rotate(CL_Angle(0, CL_AngleUnit::cl_degrees));;
}

CPhysBody Physics::FromPNG(const char* fileName)
{
	b2BodyDef bodyDef;
	bodyDef.angularDamping = 4.5f;
	bodyDef.linearDamping = 5.0f;
	bodyDef.userData = new CPhysData();
	bodyDef.active = true;
	bodyDef.type = b2_dynamicBody;

	b2PolygonShape polygonShape;
	CMarchingSquare marchingSquare(fileName);
	std::vector<CL_Vec2f> pointList = marchingSquare.DoMarch();
	int numVerticies = pointList.size();
	std::vector<b2Vec2> verticies;

	for (unsigned int i = 0; i < pointList.size(); i++)
		verticies.push_back(b2Vec2(pointList[i].x, pointList[i].y));

	polygonShape.Set(verticies.data(), verticies.size());

	b2FixtureDef fixtureDef;
	fixtureDef.friction = 0.5f;
	fixtureDef.density = 0.1f;
	fixtureDef.shape = &polygonShape;

	b2Body* body = m_world->CreateBody(&bodyDef);
	b2Fixture* fixture = body->CreateFixture(&fixtureDef);
	body->SetFixedRotation(true);
	
	return CPhysBody(body);
}

void Physics::RayCast(CL_Vec2f& startPos, CL_Vec2f& endPos)
{
	CPhysRay physRay;

	m_world->RayCast(
		&physRay,
		SwapVec2(startPos), SwapVec2(endPos)
	);

	if ( !physRay.DidHit() )
		return;

	RayData data = physRay.GetData();
	
	//try
	//{

	luabind::call_function<void>(
		g_Lua->State(), "OnRayHitBody", 
		data.body, data.position, data.normal,
		data.fraction
	);

	//}
	//catch (const luabind::error &e)
	//{
	//	g_Lua->Print( lua_tostring(e.state(), -1) );
	//}
}

void Physics::Destroy(CPhysBody& body)
{
	b2Body* bodyObject = body.Object();
	CPhysData* userData = (CPhysData*)bodyObject->GetUserData();

	m_world->DestroyBody(bodyObject);

	if (userData && userData != NULL)
		delete userData;
}

void Physics::Destroy(CPhysJoint& joint)
{
	m_world->DestroyJoint( joint.Object() );
}

b2Vec2 Physics::SwapVec2(CL_Vec2f& vector)
{
	b2Vec2 converted;
		converted.x = PixelsToMeters(vector.x);
		converted.y = PixelsToMeters(vector.y);
	return converted;
}

CL_Vec2f Physics::SwapVec2(const b2Vec2& vector)
{
	CL_Vec2f converted;
		converted.x = MetersToPixels(vector.x);
		converted.y = MetersToPixels(vector.y);
	return converted;
}

void Physics::SetGravity(CL_Vec2f& gravity)
{
	m_world->SetGravity( g_Physics->SwapVec2(gravity) );
}

CL_Vec2f Physics::GetGravity()
{
	return g_Physics->SwapVec2( m_world->GetGravity() );
}

CPhysJoint Physics::WeldJoint(CPhysBody& bodyA, CPhysBody& bodyB, CL_Vec2f& anchor, bool bCollideConnected)
{
	b2WeldJointDef jointDef;
		jointDef.Initialize( bodyA.Object(), bodyB.Object(), SwapVec2(anchor) );
		jointDef.collideConnected = bCollideConnected;
	return CPhysJoint( m_world->CreateJoint(&jointDef) ); 
}

void Physics::SetIterations(int position, int velocity)
{
	m_posIter = position;
	m_velIter = velocity;
}

void Physics::Update(float deltaTime)
{
	m_world->Step(deltaTime, m_velIter, m_posIter);
	m_world->ClearForces();
}

void Physics::Init()
{
	m_world->SetContactListener(&m_listener);
	m_world->SetContactFilter(&m_filter);
}

float Physics::MetersToPixels(float meters)
{
	return m_pixelsToMeters * meters;
}

float Physics::PixelsToMeters(float pixels)
{
	return pixels / m_pixelsToMeters;
}

void Physics::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<Physics>("Physics")
		.def("SetPixelsToMeters", &Physics::SetPixelsToMeters)
		.def("GetPixelsToMeters", &Physics::GetPixelsToMeters)
		.def("PixelsToMeters", &Physics::PixelsToMeters)
		.def("MetersToPixels", &Physics::MetersToPixels)
		.def("SetIterations", &Physics::SetIterations)
		.def("DistanceJoint", &Physics::DistanceJoint)
		.def("CreateCircle", &Physics::CreateCircle)
		.def("SetGravity", &Physics::SetGravity)
		.def("GetGravity", &Physics::GetGravity)
		.def("DebugDraw", &Physics::DebugDraw)
		.def("WeldJoint", &Physics::WeldJoint)
		.def("CreateBox", &Physics::CreateBox)
		.def("FromPNG", &Physics::FromPNG)
		.def("RayCast", &Physics::RayCast)
		.def("Destroy", (void(Physics::*)(CPhysJoint&)) &Physics::Destroy)
		.def("Destroy", (void(Physics::*)(CPhysBody&)) &Physics::Destroy)
		.def("Update", &Physics::Update)
	];

	CPhysBody::LuaBind(globals);
	CPhysJoint::LuaBind(globals);

	globals["PhysicsInstance"] = g_Physics;
}