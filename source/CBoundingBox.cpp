/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "CBoundingBox.h"
#include <algorithm>
#include "Display.h"

CBoundingBox::CBoundingBox() {}

CBoundingBox::CBoundingBox(float x, float y, float w, float h)
{
	m_position.x = x;
	m_position.y = y;
	m_size.width = w;
	m_size.height = h;
}

CBoundingBox::~CBoundingBox() {}

CBoundingBox CBoundingBox::Intersection(CBoundingBox& other)
{
    float x = std::max<float>( MinX(), other.MinX() );
	float y = std::max<float>( MinY(), other.MinY() );
    float w = std::min<float>( MaxX(), other.MaxX() ) - x;
    float h = std::min<float>( MaxY(), other.MaxY() ) - y;
	
	return CBoundingBox(x, y, w, h);
}

bool CBoundingBox::IsInside(CL_Vec2f& point)
{
	if ( point.x > MinX() && point.x < MaxX() && point.y > MinY() && point.y < MaxY() )
		return true;
	else
		return false;
}

bool CBoundingBox::OutOfBounds()
{
	if (MinY() <= 0 || MinX() <= 0 || MaxY() >= g_Display->GetH()
		|| MaxX() >= g_Display->GetW())
	{
		return true;
	}
	else
	{
		return false;
	}
}

CL_Vec2f CBoundingBox::GetCenter()
{
	return m_position + CL_Vec2f(m_size.width / 2, m_size.height / 2);
}

float CBoundingBox::MaxY()
{
	return m_position.y + m_size.height;
}

float CBoundingBox::MinY()
{
	return m_position.y;
}

float CBoundingBox::MaxX()
{
	return m_position.x + m_size.width;
}

float CBoundingBox::MinX()
{
	return m_position.x;
}

bool CBoundingBox::Intersects(CBoundingBox& other)
{
    return int( MinX() ) < int( other.MaxX() ) && int( other.MinX() ) < int( MaxX() )
		&& int( MinY() ) < int( other.MaxY() ) && int( other.MinY() ) < int( MaxY() );
}

bool CBoundingBox::IsTouching(CBoundingBox& other)
{
    return MinX() < other.MaxX() && other.MinX() < MaxX() && MinY() < other.MaxY() && other.MinY() < MaxY();
}

float CBoundingBox::GetH() const
{
	return m_size.height;
}

float CBoundingBox::GetW() const
{
	return m_size.width;
}

float CBoundingBox::GetX() const
{
	return m_position.x;
}

float CBoundingBox::GetY() const
{
	return m_position.y;
}

void CBoundingBox::SetPos(float x, float y)
{
	m_position.x = x;
	m_position.y = y;
}

void CBoundingBox::SetPos(CL_Vec2f& position)
{
	m_position.x = position.x;
	m_position.y = position.y;
}

void CBoundingBox::SetSize(float w, float h)
{
	m_size.width = w;
	m_size.height = h;
}

void CBoundingBox::SetSize(CL_Sizef& size)
{
	m_size.width = size.width;
	m_size.height = size.height;
}

CL_Vec2f& CBoundingBox::GetPos()
{
	return m_position;
}

CL_Sizef& CBoundingBox::GetSize()
{
	return m_size;
}

void CBoundingBox::SetBounds(float x, float y, float w, float h)
{
	m_position.x = x;
	m_position.y = y;
	m_size.width = w;
	m_size.height = h;
}

CL_Vec2f CBoundingBox::LineHits(CL_Vec2f& startPos, CL_Vec2f& endPos)
{
	CL_LineSegment2f rayLine(startPos, endPos);
	CL_LineSegment2f sideA( CL_Vec2f( MinX(), MinY() ), CL_Vec2f( MinX(), MaxY() ) );
	CL_LineSegment2f sideB( CL_Vec2f( MinX(), MinY() ), CL_Vec2f( MaxX(), MinY() ) );
	CL_LineSegment2f sideC( CL_Vec2f( MaxX(), MinY() ), CL_Vec2f( MaxX(), MaxY() ) );
	CL_LineSegment2f sideD( CL_Vec2f( MinX(), MaxY() ), CL_Vec2f( MaxX(), MaxY() ) );

	float distA = sideA.point_distance(startPos);
	float distB = sideB.point_distance(startPos);
	float distC = sideC.point_distance(startPos);
	float distD = sideD.point_distance(startPos);

	float minDist = std::min<float>( std::min<float>(distA, distB), std::min<float>(distC, distD) );
	bool intersects = false;

	if ( minDist == distA && rayLine.intersects(sideA, false) )
		return rayLine.get_intersection(sideA, intersects);
	else if ( minDist == distB && rayLine.intersects(sideB, false) )
		return rayLine.get_intersection(sideB, intersects);
	else if ( minDist == distC && rayLine.intersects(sideC, false) )
		return rayLine.get_intersection(sideC, intersects);
	else if ( minDist == distD && rayLine.intersects(sideD, false) )
		return rayLine.get_intersection(sideD, intersects);

	return CL_Vec2f(0, 0);
}

CL_Vec2f CBoundingBox::Translation(CBoundingBox& other)
{
    float top = ( other.MinY() - MaxY() );
    float left = ( other.MinX() - MaxX() );
    float right = ( other.MaxX() - MinX() );
    float bottom = ( other.MaxY() - MinY() );

	CL_Vec2f translation;
 
    if (std::abs(left) < right)
        translation.x = left;
    else
        translation.x = right;
 
    if (std::abs(top) < bottom)
        translation.y = top;
    else
        translation.y = bottom;

    if ( std::abs(translation.x) < std::abs(translation.y) )
        translation.y = 0;
    else
        translation.x = 0;

    return translation;
}

void CBoundingBox::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<CBoundingBox>("BoundingBox")
		.def(luabind::constructor<>())
		.def(luabind::constructor<float, float, float, float>())
		.def("Intersects", &CBoundingBox::Intersects)
		.def("IsTouching", &CBoundingBox::IsTouching)
		.def("Translation", &CBoundingBox::Translation)
		.def("Intersection", &CBoundingBox::Intersection)
		.def("OutOfBounds", &CBoundingBox::OutOfBounds)
		.def("LineHits", &CBoundingBox::LineHits)
		.def("SetBounds", &CBoundingBox::SetBounds)
		.def("IsInside", &CBoundingBox::IsInside)
		.def("GetCenter", &CBoundingBox::GetCenter)
		.def("GetSize", &CBoundingBox::GetSize)
		.def("SetSize", (void(CBoundingBox::*)(CL_Sizef&)) &CBoundingBox::SetSize)
		.def("GetPos", &CBoundingBox::GetPos)
		.def("SetPos", (void(CBoundingBox::*)(CL_Vec2f&)) &CBoundingBox::SetPos)
		.def("GetW", &CBoundingBox::GetW)
		.def("GetH", &CBoundingBox::GetH)
		.def("GetX", &CBoundingBox::GetX)
		.def("GetY", &CBoundingBox::GetY)
		.def("MaxY", &CBoundingBox::MaxY)
		.def("MinY", &CBoundingBox::MinY)
		.def("MaxX", &CBoundingBox::MaxX)
		.def("MinX", &CBoundingBox::MinX)
	];
}