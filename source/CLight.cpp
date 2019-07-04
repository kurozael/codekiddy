/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "Display.h"
#include "Lights.h"
#include "CLight.h"
#include "Util.h"

CLight::CLight()
{
	m_isOn = true;
	m_origin.x = 0.5;
	m_origin.y = 0.5;
	m_uniqueID = "";
}

CLight::CLight(CL_Image& image)
{
	m_isOn = true;
	m_image = image;
	m_origin.x = 0.5;
	m_origin.y = 0.5;
	m_uniqueID = "";
}

CLight::~CLight() {}

void CLight::SetPos(CL_Vec2f& position)
{
	m_position.x = position.x;
	m_position.y = position.y;

	UpdateRect();
}

CL_Image& CLight::GetImage()
{
	return m_image;
}

void CLight::SetMaterial(const char* materialPath)
{
	std::string newMaterialPath = Util::FixFilePath(materialPath, "materials/", ".png");
	CL_GraphicContext graphics = g_Display->Graphics().Object();

	m_image = CL_Image(graphics, newMaterialPath);
}

void CLight::SetUniqueID(const std::string& uniqueID)
{
	m_uniqueID = uniqueID;
}

std::string CLight::GetUniqueID()
{
	return m_uniqueID;
}

void CLight::UpdateRect()
{
	float originX = m_size.width * m_origin.x;
	float originY = m_size.height * m_origin.y;

	m_rect.bottom = (m_position.y + m_size.height) - originY;
	m_rect.right = (m_position.x + m_size.width) - originX;
	m_rect.left = m_position.x - originX;
	m_rect.top = m_position.y - originY;
}

void CLight::SetOrigin(float x, float y)
{
	m_origin.x = std::max<float>(std::min<float>(x, 1), 0);
	m_origin.y = std::max<float>(std::min<float>(y, 1), 0);
	UpdateRect();
}

CL_Vec2f& CLight::GetOrigin()
{
	return m_origin;
}

CL_Vec2f& CLight::GetPos()
{
	return m_position;
}

void CLight::SetColor(CL_Colorf& color)
{
	m_color = color;
}

CL_Colorf& CLight::GetColor()
{
	return m_color;
}

void CLight::SetSize(float w, float h)
{
	m_size.width = w;
	m_size.height = h;

	UpdateRect();
}

CL_Sizef& CLight::GetSize()
{
	return m_size;
}

void CLight::SetIntensity(float intensity)
{
	if (intensity > 1.0f)
		intensity = 1.0f;
	else if (intensity < 0.0f)
		intensity = 0.0f;

	m_intensity = intensity;
}

float CLight::GetIntensity()
{
	return m_intensity;
}

CL_Rectf& CLight::GetRect()
{
	return m_rect;
}

void CLight::SetAngle(CL_Angle& angle)
{
	m_angle = angle;
}

CL_Angle& CLight::GetAngle()
{
	return m_angle;
}

void CLight::TurnOn()
{
	m_isOn = true;
}

void CLight::TurnOff()
{
	m_isOn = false;
}

void CLight::Remove()
{
	g_Lights->Remove(m_uniqueID);
}

bool CLight::IsOn()
{
	return m_isOn;
}

void CLight::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<CLight>("Light")
		.def("IsOn", &CLight::IsOn)
		.def("SetPos", &CLight::SetPos)
		.def("GetPos", &CLight::GetPos)
		.def("Remove", &CLight::Remove)
		.def("TurnOn", &CLight::TurnOn)
		.def("TurnOff", &CLight::TurnOff)
		.def("SetColor", &CLight::SetColor)
		.def("GetColor", &CLight::GetColor)
		.def("SetAngle", &CLight::SetAngle)
		.def("GetAngle", &CLight::GetAngle)
		.def("SetSize", &CLight::SetSize)
		.def("GetSize", &CLight::GetSize)
		.def("SetOrigin", &CLight::SetOrigin)
		.def("GetOrigin", &CLight::GetOrigin)
		.def("GetUniqueID", &CLight::GetUniqueID)
		.def("SetMaterial", &CLight::SetMaterial)
		.def("GetIntensity", &CLight::GetIntensity)
		.def("SetIntensity", &CLight::SetIntensity)
	];
}