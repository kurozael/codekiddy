/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "Camera.h"
#include "Display.h"

Camera::Camera()
{
	m_bRectDirty = true;
	m_worldPos.x = -1.0f;
	m_worldPos.y = -1.0f;
	m_stack = 0;
	m_scale = -1.0f;
	m_angle = CL_Angle(-1.0f, CL_AngleUnit::cl_degrees);
	Reset();
}

Camera::~Camera() {}

bool Camera::IsInScreenBounds(CL_Vec2f& position)
{
	return m_screenBounds.contains(position);
}

void Camera::SetScreenBounds(int x, int y, int w, int h)
{
	m_screenBounds.bottom = y + h;
	m_screenBounds.right = x + w;
	m_screenBounds.left = x;
	m_screenBounds.top = y;

	AdjustPosition();
}

void Camera::SetWorldBounds(int x, int y, int w, int h)
{
	m_worldBounds.bottom = y + h;
	m_worldBounds.right = x + w;
	m_worldBounds.left = x;
	m_worldBounds.top = y;
	
	AdjustPosition();
}

CL_Sizef Camera::GetVisibleArea(bool bNoScale)
{
	float scale = (!bNoScale ? m_scale : 1);
	float sin = std::abs(m_sin);
	float cos = std::abs(m_cos);
	CL_Sizef size;

	size.width = GetW() / m_scale;
	size.height = GetH() / m_scale;
	size.width = cos * size.width + sin * size.height;
	size.height = sin * size.width + cos * size.height;
	size.width = std::min<float>(size.width, (float)m_worldBounds.get_width());
	size.height = std::min<float>(size.height, (float)m_worldBounds.get_height());

	return size;
}

CL_Rect& Camera::GetScreenBounds()
{
	return m_screenBounds;
}

CL_Rectf& Camera::GetWorldRect()
{
	if (m_bRectDirty)
	{
		CL_Sizef visibleArea = GetVisibleArea();
			float x = m_worldPos.x - (visibleArea.width * 0.5f);
			float y = m_worldPos.y - (visibleArea.height * 0.5f);
		m_worldRect = CL_Rectf(x, y, x + visibleArea.width, y + visibleArea.height);
		m_bRectDirty = false;
	}

	return m_worldRect;
}

void Camera::AdjustPosition()
{
	CL_Sizef visibleArea = GetVisibleArea();
	
	float worldWidth = (float)m_worldBounds.get_width();
	float worldHeight = (float)m_worldBounds.get_height();
	float halfWW = visibleArea.width * 0.5f;
	float halfWH = visibleArea.height * 0.5f;
	float left = m_worldBounds.left + halfWW;
	float right = m_worldBounds.left + worldWidth - halfWW;
	float top = m_worldBounds.top + halfWH;
	float bottom = m_worldBounds.top + worldHeight - halfWH;

	m_worldPos.x = floor(cl_clamp(m_worldPos.x, left, right));
	m_worldPos.y = floor(cl_clamp(m_worldPos.y, top, bottom));
	m_bRectDirty = true;
}

CL_Vec2f Camera::ToScreen(CL_Vec2f& position)
{
	CL_Vec2f screenPos;
	float x = position.x - m_worldPos.x;
	float y = position.y - m_worldPos.y;

	screenPos.x = -m_cos*x + m_sin*y;
	screenPos.y = -m_sin*x - m_cos*y;
	screenPos.x = m_worldPos.x - (screenPos.x / m_scale + m_screenBounds.left);
	screenPos.y = m_worldPos.y - (screenPos.y / m_scale + m_screenBounds.top);

	return screenPos - GetWorldRect().get_top_left();
}

CL_Vec2f Camera::ToWorld(CL_Vec2f& position)
{
	CL_Vec2f worldPos;
	float halfW = m_screenBounds.get_width() * 0.5f;
	float halfH = m_screenBounds.get_height() * 0.5f;
	float x = (position.x - halfW - m_screenBounds.left) / m_scale;
	float y = (position.y - halfH - m_screenBounds.top) / m_scale;

	worldPos.x = m_cos*x - m_sin*y;
	worldPos.y = m_sin*x + m_cos*y;
	worldPos.x = worldPos.x + m_worldPos.x;
	worldPos.y = worldPos.y + m_worldPos.y;

	return worldPos;
}

void Camera::AdjustScale()
{
  CL_Sizef visibleArea = GetVisibleArea(true);

  float sx = visibleArea.width / m_worldBounds.get_width();
  float sy = visibleArea.height / m_worldBounds.get_height();
  float newScale = std::max<float>(sx, sy);

  m_scale = std::max<float>(m_scale, newScale);
  m_bRectDirty = true;
}

void Camera::SetAngle(float angle)
{
	if (m_angle.to_degrees() == angle)
		return;

	m_angle.set_degrees(angle);
	m_sin = std::sin(m_angle.to_radians());
	m_cos = std::cos(m_angle.to_radians());

	AdjustScale();
	AdjustPosition();
}

CL_Vec2f& Camera::GetWorldPos()
{
	return m_worldPos;
}

CL_Vec2f& Camera::GetDrawPos()
{
	return m_worldPos;
}

void Camera::SetWorldPos(CL_Vec2f& position)
{
	float x = floor(position.x);
	float y = floor(position.y);

	if (m_worldPos.x == x && m_worldPos.y == y)
		return;

	m_worldPos.x = x;
	m_worldPos.y = y;
	
	AdjustPosition();
}

void Camera::SetScale(float scale)
{
	if (m_scale == scale)
		return;

	if (scale < 0.0f)
		scale = 0.0f;

	m_scale = scale;

	AdjustScale();
	AdjustPosition();
}

float Camera::GetAngle() { return m_angle.to_degrees(); }

float Camera::GetScale()
{
	return m_scale;
}

CL_Vec2f& Camera::GetPos()
{
	if (m_enabled)
	{
		return GetDrawPos();
	}
	else
	{
		return m_origin;
	}
}

bool Camera::IsEnabled()
{
	return m_enabled;
}

void Camera::ClearStack() { m_stack = 0; }

void Camera::Begin()
{
	CL_GraphicContext graphics = g_Display->Graphics().Object();

	if (!m_enabled)
	{
		graphics.push_cliprect(m_screenBounds);
		graphics.push_modelview();
		graphics.mult_scale(m_scale, m_scale);
		graphics.mult_translate(
			((GetW() * 0.5f) + m_screenBounds.left) / m_scale,
			((GetH() * 0.5f) + m_screenBounds.top) / m_scale
		);
		graphics.mult_rotate(m_angle);
		graphics.mult_translate(-m_worldPos.x, -m_worldPos.y);

		m_enabled = true;
		m_stack++;
	}
}

void Camera::Finish()
{
	CL_GraphicContext graphics = g_Display->Graphics().Object();
	
	if (m_enabled)
	{
		graphics.pop_modelview();
		m_enabled = false;
	}

	if (m_stack > 0)
	{
		graphics.pop_cliprect();
		m_stack--;
	}
}

void Camera::Reset()
{
	int screenHeight = g_Display->GetH();
	int screenWidth = g_Display->GetW();

	SetScreenBounds(0, 0, screenWidth, screenHeight);
	SetWorldBounds(0, 0, screenWidth, screenHeight);
	SetWorldPos(CL_Vec2f(0.0f, 0.0f));
	SetScale(1.0f);
	SetAngle(0.0f);
}

int Camera::GetH()
{
	return m_screenBounds.get_height();
}

int Camera::GetW()
{
	return m_screenBounds.get_width();
}

int Camera::GetX()
{
	if (m_enabled)
		return m_screenBounds.get_top_left().x;

	return int(m_origin.x);
}

int Camera::GetY()
{
	if (m_enabled)
		return int(floor(m_screenBounds.get_top_left().y));

	return int(m_origin.y);
}

void Camera::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<Camera>("Camera")
		.def("IsInScreenBounds", &Camera::IsInScreenBounds)
		.def("SetScreenBounds", &Camera::SetScreenBounds)
		.def("SetWorldBounds", &Camera::SetWorldBounds)
		.def("GetWorldPos", &Camera::GetWorldPos)
		.def("GetWorldRect", &Camera::GetWorldRect)
		.def("GetVisibleArea", &Camera::GetVisibleArea)
		.def("SetWorldPos", &Camera::SetWorldPos)
		.def("GetDrawPos", &Camera::GetDrawPos)
		.def("IsEnabled", &Camera::IsEnabled)
		.def("SetAngle", &Camera::SetAngle)
		.def("GetAngle", &Camera::GetAngle)
		.def("ToScreen", &Camera::ToScreen)
		.def("ToWorld", &Camera::ToWorld)
		.def("GetScale", &Camera::GetScale)
		.def("SetScale", &Camera::SetScale)
		.def("GetPos", &Camera::GetPos)
		.def("Finish", &Camera::Finish)
		.def("Begin", &Camera::Begin)
		.def("Reset", &Camera::Reset)
		.def("GetH", &Camera::GetH)
		.def("GetW", &Camera::GetW)
		.def("GetX", &Camera::GetX)
		.def("GetY", &Camera::GetY)
	];

	globals["CameraInstance"] = g_Camera;
}