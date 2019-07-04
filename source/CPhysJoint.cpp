/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "CPhysJoint.h"
#include "Physics.h"

CPhysJoint::CPhysJoint(b2Joint* joint)
{
	m_object = joint;
}

CPhysJoint::CPhysJoint() {}

CPhysJoint::~CPhysJoint() {}

CPhysBody CPhysJoint::GetBodyA()
{
	return CPhysBody( m_object->GetBodyA() );
}

CPhysBody CPhysJoint::GetBodyB()
{
	return CPhysBody( m_object->GetBodyB() );
}

CL_Vec2f CPhysJoint::GetAnchorA()
{
	return g_Physics->SwapVec2( m_object->GetAnchorA() );
}

CL_Vec2f CPhysJoint::GetAnchorB()
{
	return g_Physics->SwapVec2( m_object->GetAnchorB() );
}

void CPhysJoint::Destroy()
{
	g_Physics->Destroy(*this);
}

b2Joint* CPhysJoint::Object()
{
	return m_object;
}

void CPhysJoint::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<CPhysJoint>("PhysJoint")
		.def("GetAnchorA", &CPhysJoint::GetAnchorA)
		.def("GetAnchorB", &CPhysJoint::GetAnchorB)
		.def("GetBodyA", &CPhysJoint::GetBodyA)
		.def("GetBodyB", &CPhysJoint::GetBodyB)
		.def("Destroy", &CPhysJoint::Destroy)
	];
}