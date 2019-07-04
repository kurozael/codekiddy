/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "Render.h"
#include "Display.h"
#include "Util.h"
#include "Game.h"

Render::Render()
{
	m_color = CL_Colorf::transparent;
	m_shading = false;
	m_blending = false;
	m_blendMode.enable_blending(true);
}

Render::~Render() {}

void Render::OverrideColor(CL_Colorf& color)
{
	m_color = color;
}

void Render::DrawPixel(float x, float y, CL_Colorf& color)
{
	if (m_color != CL_Colorf::transparent)
		color = m_color;
	
	CL_Draw::point(g_Display->Graphics().Object(), x, y, color);
}

void Render::DrawLine(float x1, float y1, float x2, float y2, CL_Colorf& color)
{
	if (m_color != CL_Colorf::transparent)
		color = m_color;

	CL_Draw::line(g_Display->Graphics().Object(), x1, y1, x2, y2, color);
}

void Render::DrawBox(float x, float y, float w, float h, CL_Colorf& color)
{
	if (m_color != CL_Colorf::transparent)
		color = m_color;

	CL_Draw::box(g_Display->Graphics().Object(), x, y, x + w, y + h, color);
}

void Render::DrawFill(float x, float y, float w, float h, CL_Colorf& color)
{
	if (m_color != CL_Colorf::transparent)
		color = m_color;

	CL_Draw::fill(g_Display->Graphics().Object(), x, y, x + w, y + h, color);
}

void Render::DrawTexture(CL_Texture& texture, float x, float y, float w, float h, CL_Colorf& color)
{
	CL_GraphicContext& gc = g_Display->Graphics().Object();
	CL_Rectf textureCoords(0.0f, 0.0f, 1.0f, 1.0f);
	CL_Rectf quadRect(x, y, x + w, y + h);

	if (m_color != CL_Colorf::transparent)
		color = m_color;
	
	CL_Vec2f quadPositions[6] =
	{
		CL_Vec2f(quadRect.left, quadRect.top),
		CL_Vec2f(quadRect.right, quadRect.top),
		CL_Vec2f(quadRect.left, quadRect.bottom),
		CL_Vec2f(quadRect.right, quadRect.top),
		CL_Vec2f(quadRect.left, quadRect.bottom),
		CL_Vec2f(quadRect.right, quadRect.bottom)
	};

	CL_Vec2f quadCoords[6] =
	{
		CL_Vec2f(textureCoords.left, textureCoords.top),
		CL_Vec2f(textureCoords.right, textureCoords.top),
		CL_Vec2f(textureCoords.left, textureCoords.bottom),
		CL_Vec2f(textureCoords.right, textureCoords.top),
		CL_Vec2f(textureCoords.left, textureCoords.bottom),
		CL_Vec2f(textureCoords.right, textureCoords.bottom)
	};

	// A necessary evil.
	if (!m_shading)
	{
		gc.set_program_object(
			CL_StandardProgram::cl_program_single_texture
		);
	}

	gc.set_texture(0, texture);
	
	CL_PrimitivesArray primArray(gc);
		primArray.set_attributes(CL_StandardAttributeIndex::cl_attrib_position, quadPositions);
		primArray.set_attributes(CL_StandardAttributeIndex::cl_attrib_texture_position, quadCoords);
		primArray.set_attribute(CL_StandardAttributeIndex::cl_attrib_color, color);
	gc.draw_primitives(cl_triangles, 6, primArray);

	gc.reset_texture(0);
}

void Render::SetShading(bool bIsShading)
{
	m_shading = bIsShading;
}

bool Render::IsShading()
{
	return m_shading;
}

void Render::DrawImage(CL_Image& image, float x, float y, float w, float h, CL_Colorf& color)
{
	if (m_color != CL_Colorf::transparent)
		color = m_color;

	image.set_color(color);
	image.draw( g_Display->Graphics().Object(), CL_Rectf(x, y, x + w, y + h) );
}

void Render::DrawGradientFill(float x, float y, float w, float h, CL_Gradient& gradient)
{
	if (m_color != CL_Colorf::transparent)
		gradient = CL_Gradient(m_color, m_color);

	CL_Draw::gradient_fill(g_Display->Graphics().Object(), x, y, x + w, y + h, gradient);
}

void Render::DrawCircle(float x, float y, float radius, CL_Colorf& color)
{
	if (m_color != CL_Colorf::transparent)
		color = m_color;

	CL_Draw::circle(g_Display->Graphics().Object(), x, y, radius, color);
}

void Render::DrawGradientCircle(float x, float y, float radius, CL_Gradient& gradient)
{
	if (m_color != CL_Colorf::transparent)
		gradient = CL_Gradient(m_color, m_color);

	CL_Draw::gradient_circle(g_Display->Graphics().Object(), CL_Pointf(x, y), radius, gradient);
}

CL_Texture Render::GetTexture(const char* materialPath)
{
	std::string newMaterialPath = Util::FixFilePath(materialPath, "materials/", ".png");

	if (g_Files->IsLocal(newMaterialPath.c_str()))
		return CL_Texture(g_Display->Graphics().Object(), newMaterialPath);
	else
		return CL_Texture(g_Display->Graphics().Object(), newMaterialPath, g_Game->GetVD());
}

CL_Image Render::GetImage(const char* materialPath)
{
	std::string newMaterialPath = Util::FixFilePath(materialPath, "materials/", ".png");

	if (g_Files->IsLocal(newMaterialPath.c_str()))
		return CL_Image(g_Display->Graphics().Object(), newMaterialPath);
	else
		return CL_Image(g_Display->Graphics().Object(), newMaterialPath, g_Game->GetVD());
}

void Render::SetBlendFunc(int source, int destination)
{
	
	m_blendMode.set_blend_function(
		(CL_BlendFunc)source, (CL_BlendFunc)destination,
		(CL_BlendFunc)source, (CL_BlendFunc)destination
	);

	if (m_blending)
		g_Display->Graphics().Object().set_blend_mode(m_blendMode);
}

void Render::SetBlendColor(CL_Colorf& color)
{
	m_blendMode.set_blend_color(color);

	if (m_blending)
		g_Display->Graphics().Object().set_blend_mode(m_blendMode);
}

void Render::StartBlending()
{
	g_Display->Graphics().Object().set_blend_mode(m_blendMode);
	m_blending = true;
}

void Render::StopBlending()
{
	g_Display->Graphics().Object().set_blend_mode(m_blendDefault);
	m_blending = false;
}

void Render::SetDepthMask(bool bDepthMask)
{
	glDepthMask(bDepthMask);
}

bool Render::IsBlending()
{
	return (g_Display->Graphics().Object().get_blend_mode() == m_blendMode);
}

void Render::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<Render>("Render")
		.def("OverrideColor", &Render::OverrideColor)
		.def("SetDepthMask", &Render::SetDepthMask)
		.def("DrawPixel", &Render::DrawPixel)
		.def("DrawLine", &Render::DrawLine)
		.def("DrawBox", &Render::DrawBox)
		.def("DrawFill", &Render::DrawFill)
		.def("DrawTexture", &Render::DrawTexture)
		.def("DrawImage", &Render::DrawImage)
		.def("DrawGradientFill", &Render::DrawGradientFill)
		.def("DrawCircle", &Render::DrawCircle)
		.def("DrawGradientCircle", &Render::DrawGradientCircle)
		.def("SetBlendFunc", &Render::SetBlendFunc)
		.def("SetBlendColor", &Render::SetBlendColor)
		.def("StartBlending", &Render::StartBlending)
		.def("StopBlending", &Render::StopBlending)
		.def("IsBlending", &Render::IsBlending)
		.def("IsShading", &Render::IsShading)
		.def("GetTexture", &Render::GetTexture)
		.def("GetImage", &Render::GetImage)
	];

	globals["RenderInstance"] = g_Render;
}