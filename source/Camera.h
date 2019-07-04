/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CAMERA_H
#define CAMERA_H

#include <ClanLib/core.h>
#include "CNonCopyable.h"
#include "CSingleton.h"
#include "Lua.h"

class Camera : public CNonCopyable
{
public:
	bool IsInScreenBounds(CL_Vec2f& position);
	void SetScreenBounds(int x, int y, int w, int h);
	void SetWorldBounds(int x, int y, int w, int h);
	CL_Rect& GetScreenBounds();
	CL_Vec2f& GetWorldPos();
	CL_Rectf& GetWorldRect();
	CL_Vec2f& GetDrawPos();
	void SetWorldPos(CL_Vec2f& position);
	void AdjustPosition();
	void AdjustScale();
	CL_Sizef GetVisibleArea(bool bNoScale = false);
	void SetScale(float scale);
	void SetAngle(float angle);
	CL_Vec2f ToScreen(CL_Vec2f& position);
	CL_Vec2f ToWorld(CL_Vec2f& position);
	float GetAngle();
	void ClearStack();
	bool IsEnabled();
	float GetScale();
	CL_Vec2f& GetPos();
	void Begin();
	void Finish();
	void Reset();
	int GetH();
	int GetW();
	int GetX();
	int GetY();
	void LuaBind(luabind::object& globals);
	~Camera();
	Camera();
protected:
	CL_Vec2f m_worldPos;
	CL_Vec2f m_origin;
	CL_Rectf m_worldRect;
	CL_Rect m_screenBounds;
	CL_Rect m_worldBounds;
	bool m_bRectDirty;
	bool m_enabled;
	float m_scale;
	CL_Angle m_angle;
	float m_sin;
	float m_cos;
	int m_stack;
};

#define g_Camera CSingleton<Camera>::Instance()

#endif