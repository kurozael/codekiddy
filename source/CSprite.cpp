/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "CSprite.h"
#include "Render.h"
#include "Events.h"
#include "Game.h"
#include "Time.h"
#include "Util.h"
#include "Files.h"

CSprite::CSprite()
{
	m_sprite = CL_Sprite(g_Display->Graphics().Object(), CL_SpriteDescription());
	m_addingFrames = false;
}

CSprite::CSprite(const char* materialPath)
{
	std::string newMaterialPath = Util::FixFilePath(materialPath, "materials/", ".png");
	
	if (g_Files->IsLocal(newMaterialPath.c_str()))
		m_sprite = CL_Sprite(g_Display->Graphics().Object(), newMaterialPath);
	else
		m_sprite = CL_Sprite(g_Display->Graphics().Object(), newMaterialPath, g_Game->GetVD());

	m_addingFrames = false;
}

CSprite::~CSprite() {}

int CSprite::GetH() const
{
	return m_sprite.get_height();
}

int CSprite::GetW() const
{
	return m_sprite.get_width();
}

void CSprite::Update()
{
	m_sprite.update();
}

void CSprite::Draw(float x, float y, float w, float h, CL_Angle& angle, CL_Colorf& color)
{
	m_sprite.set_scale( w / m_sprite.get_width(), h / m_sprite.get_height() );
	m_sprite.set_angle(angle);
	m_sprite.set_color(color);
	m_sprite.set_rotation_hotspot(CL_Origin::origin_center);
	m_sprite.draw(g_Display->Graphics().Object(), x, y);
}

void CSprite::DrawAroundPoint(float x, float y, float w, float h, CL_Angle& angle, CL_Colorf& color, CL_Vec2f& point)
{
	m_sprite.set_scale( w / m_sprite.get_width(), h / m_sprite.get_height() );
	m_sprite.set_angle(angle);
	m_sprite.set_color(color);
	m_sprite.set_rotation_hotspot(CL_Origin::origin_center, int(point.x), int(point.y));
	m_sprite.draw(g_Display->Graphics().Object(), x, y);
}

void CSprite::StartFrames()
{
	if (!m_addingFrames)
	{
		m_spriteDesc = CL_SpriteDescription();
		m_addingFrames = true;
	}
}

void CSprite::AddSheet(const char* materialPath, int x, int y, int w, int h, int framesX, int framesY, int spacingX, int spacingY, int skipFrames)
{
	std::string newMaterialPath = Util::FixFilePath(materialPath, "materials/", ".png");

	if (m_addingFrames)
	{
		CL_Texture texture = g_Render->GetTexture(newMaterialPath.c_str());
		m_spriteDesc.add_gridclipped_frames(texture, x, y, w, h, framesX, framesY, skipFrames, spacingX, spacingY);
	}
}

void CSprite::AddImage(const char* materialPath)
{
	std::string newMaterialPath = Util::FixFilePath(materialPath, "materials/", ".png");

	if (m_addingFrames)
	{
		if (g_Files->IsLocal(newMaterialPath.c_str()))
			m_spriteDesc.add_frame(newMaterialPath);
		else
			m_spriteDesc.add_frame(newMaterialPath, g_Game->GetVD());
	}
}

void CSprite::FinishFrames()
{
	if (m_addingFrames)
	{
		m_sprite = CL_Sprite(g_Display->Graphics().Object(), m_spriteDesc);
		m_addingFrames = false;
	}
}

void CSprite::SetLooping(bool isLooping)
{
	m_sprite.set_play_loop(isLooping);
}

void CSprite::SetPingPong(bool isPingPng)
{
	m_sprite.set_play_pingpong(isPingPng);
}

bool CSprite::GetLooping()
{
	return m_sprite.is_play_loop();
}

bool CSprite::GetPingPong()
{
	return m_sprite.is_play_pingpong();
}

void CSprite::CopyFrames(CSprite& sprite)
{
	m_sprite.set_image_data( sprite.Object() );
}

void CSprite::CopyModes(CSprite& sprite)
{
	m_sprite.set_play_pingpong( sprite.Object().is_play_pingpong() );
	m_sprite.set_play_loop( sprite.Object().is_play_loop() );
}

void CSprite::SetRotateSpot(CL_Origin origin)
{
	m_sprite.set_rotation_hotspot(origin);
}

void CSprite::SetTranslateSpot(CL_Origin origin)
{
	m_sprite.set_alignment(origin);
}

void CSprite::SetDelay(float seconds)
{
	m_sprite.set_delay( int(seconds * 1000.0f) );
}

CL_Sprite& CSprite::Object()
{
	return m_sprite;
}

void CSprite::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<CSprite>("Sprite")
		.def(luabind::constructor<>())
		.def(luabind::constructor<const char*>())
		.def("DrawAroundPoint", &CSprite::DrawAroundPoint)
		.def("SetTranslateSpot", &CSprite::SetTranslateSpot)
		.def("SetRotateSpot", &CSprite::SetRotateSpot)
		.def("StartFrames", &CSprite::StartFrames)
		.def("FinishFrames", &CSprite::FinishFrames)
		.def("SetPingPong", &CSprite::SetPingPong)
		.def("GetPingPong", &CSprite::GetPingPong)
		.def("SetLooping", &CSprite::SetLooping)
		.def("GetLooping", &CSprite::GetLooping)
		.def("CopyFrames", &CSprite::CopyFrames)
		.def("CopyModes", &CSprite::CopyModes)
		.def("SetDelay", &CSprite::SetDelay)
		.def("AddSheet", &CSprite::AddSheet)
		.def("AddImage", &CSprite::AddImage)
		.def("GetW", &CSprite::GetW)
		.def("GetH", &CSprite::GetH)
		.def("Update", &CSprite::Update)
		.def("Draw", &CSprite::Draw)
	];
}