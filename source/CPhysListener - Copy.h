/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CPHYSLISTENER_H
#define CPHYSLISTENER_H

#include <ClanLib/core.h>
#include <Box2D/Box2D.h>

class CPhysListener : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
	CPhysListener();
	~CPhysListener();
};

#endif