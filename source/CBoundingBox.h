/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CBOUNDINGBOX_H
#define CBOUNDINGBOX_H

#include <ClanLib/core.h>
#include "Lua.h"

class CBoundingBox
{
public:
	CBoundingBox();
	CBoundingBox(float x, float y, float w = 0, float = 0);
	~CBoundingBox();
	bool Intersects(CBoundingBox& other);
	bool IsTouching(CBoundingBox& other);
	bool IsInside(CL_Vec2f& point);
	CL_Vec2f LineHits(CL_Vec2f& startPos, CL_Vec2f& endPos);
	CBoundingBox Intersection(CBoundingBox& other);
	CL_Vec2f Translation(CBoundingBox& other);
	CL_Vec2f GetCenter();
	bool OutOfBounds();
	float GetH() const;
	float GetW() const;
	float GetX() const;
	float GetY() const;
	void SetPos(float x, float y);
	void SetPos(CL_Vec2f& position);
	void SetSize(float w, float h);
	void SetSize(CL_Sizef& size);
	void SetBounds(float x, float y, float w, float h);
	float MaxY();
	float MinY();
	float MaxX();
	float MinX();
	CL_Sizef& GetSize();
	CL_Vec2f& GetPos();
	static void LuaBind(luabind::object& globals);
private:
	CL_Vec2f m_position;
	CL_Sizef m_size;
};

#endif