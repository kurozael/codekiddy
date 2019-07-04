/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "Lights.h"
#include "Display.h"
#include "Game.h"

Lights::Lights()
{
	m_ambience = CL_Colorf(0.0f, 0.0f, 0.0f, 0.8f);
}

Lights::~Lights() {}

CLight& Lights::Add(std::string uniqueID, CL_Colorf& color, CL_Vec2f& position, float w, float h, float intensity)
{
	CLight light(m_lightImage);
		light.SetPos(position);
		light.SetSize(w, h);
		light.SetColor(color);
		light.SetUniqueID(uniqueID);
		light.SetIntensity(intensity);
	m_lightMap[uniqueID] = light;

	return m_lightMap[uniqueID];
}

void Lights::Remove(std::string uniqueID)
{
	m_lightMap.erase(uniqueID);
}

void Lights::SetAmbience(CL_Colorf& ambience)
{
	m_ambience = ambience;
}

CL_Colorf& Lights::GetAmbience()
{
	return m_ambience;
}

void Lights::Begin()
{
	CL_GraphicContext graphics = g_Display->Graphics().Object();

	for (LightMap::iterator it = m_lightMap.begin(); it != m_lightMap.end(); it++)
	{
		if ( !it->second.IsOn() )
			continue;

		CL_Rectf rect( it->second.GetRect() );
		g_Camera->ToWorldPos(rect);

		graphics.push_modelview();
			CL_Sizef size = it->second.GetSize();
			CL_Image image = it->second.GetImage();
			CL_Vec2f origin = it->second.GetOrigin();

			float originX = rect.left + (size.width * origin.x);
			float originY = rect.top + (size.height * origin.y);

			graphics.mult_translate(originX, originY);
				graphics.mult_rotate( it->second.GetAngle() );
			graphics.mult_translate(-originX, -originY);

			image.set_color( it->second.GetColor() );
			image.draw(graphics, rect);
		graphics.pop_modelview();
	}
}

void Lights::Finish()
{
	CL_GraphicContext graphics = g_Display->Graphics().Object();

	int screenWidth = g_Display->GetW();
	int screenHeight = g_Display->GetH();

	CL_FrameBuffer& prevBuffer = graphics.get_write_frame_buffer();

	graphics.set_frame_buffer(m_frameBuffer);
	graphics.clear(m_ambience);

	for (LightMap::iterator it = m_lightMap.begin(); it != m_lightMap.end(); it++)
	{
		if ( !it->second.IsOn() )
			continue;

		CL_BlendMode blender;

		blender.enable_blending(true);
		blender.set_blend_function(
			cl_blend_zero, cl_blend_one_minus_src_alpha,
			cl_blend_zero, cl_blend_one_minus_src_alpha
		);
		graphics.set_blend_mode(blender);

		CL_Rectf rect( it->second.GetRect() );
		g_Camera->ToWorldPos(rect);

		graphics.push_modelview();
			CL_Sizef size = it->second.GetSize();
			CL_Image image = it->second.GetImage();
			CL_Vec2f origin = it->second.GetOrigin();

			float originX = rect.left + (size.width * origin.x);
			float originY = rect.top + (size.height * origin.y);

			graphics.mult_translate(originX, originY);
				graphics.mult_rotate( it->second.GetAngle() );
			graphics.mult_translate(-originX, -originY);

			image.set_color( CL_Colorf( 1.0f, 1.0f, 1.0f, it->second.GetIntensity() ) );
			image.draw(graphics, rect);
		graphics.pop_modelview();
		graphics.reset_blend_mode();
	}

	if (!prevBuffer.is_null())
		graphics.set_frame_buffer(prevBuffer);
	else
		graphics.reset_frame_buffer();

	graphics.set_texture(0, m_lightMask);
		CL_Draw::texture( graphics, CL_Rect( 0, 0, g_Display->GetW(), g_Display->GetH() ), CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f) );
		graphics.pop_modelview();
	graphics.reset_texture(0);
}

void Lights::Init()
{
	CL_GraphicContext graphics = g_Display->Graphics().Object();	

	m_lightMask = CL_Texture(graphics, g_Display->GetW(), g_Display->GetH());
	m_lightImage = CL_Image(graphics, "materials/lights/spotlight.png", g_Game->GetVD());
	m_frameBuffer = CL_FrameBuffer(graphics);
	m_frameBuffer.attach_color_buffer(0, m_lightMask);
}

void Lights::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<Lights>("Lights")
		.def("SetAmbience", &Lights::SetAmbience)
		.def("GetAmbience", &Lights::GetAmbience)
		.def("Remove", &Lights::Remove)
		.def("Begin", &Lights::Begin)
		.def("Finish", &Lights::Finish)
		.def("Add", &Lights::Add)
	];

	globals["lights"] = g_Lights;
}