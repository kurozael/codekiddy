/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CLIGHT_H
#define CLIGHT_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "Lua.h"

class CLight
{
public:
	CLight();
	~CLight();
	CLight(CL_Image& image);
	void SetPos(CL_Vec2f& position);
	void SetSize(float w, float h);
	void SetColor(CL_Colorf& color);
	void SetAngle(CL_Angle& angle);
	void SetOrigin(float x, float y);
	void SetMaterial(const char* materialPath);
	void SetIntensity(float intensity);
	CL_Vec2f& GetOrigin();
	CL_Image& GetImage();
	CL_Rectf& GetRect();
	CL_Vec2f& GetPos();
	CL_Colorf& GetColor();
	CL_Angle& GetAngle();
	CL_Sizef& GetSize();
	void SetUniqueID(const std::string& uniqueID);
	std::string GetUniqueID();
	float GetIntensity();
	float GetRadius();
	void UpdateRect();
	void TurnOn();
	void TurnOff();
	void Remove();
	bool IsOn();
	static void LuaBind(luabind::object& globals);
protected:
	std::string m_uniqueID;
	float m_intensity;
	CL_Vec2f m_origin;
	CL_Colorf m_color;
	CL_Vec2f m_position;
	CL_Image m_image;
	CL_Angle m_angle;
	CL_Rectf m_rect;
	CL_Sizef m_size;
	bool m_isOn;
};

#endif