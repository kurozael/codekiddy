/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "CFrameBuffer.h"
#include "Display.h"
#include "Render.h"
#include <vector>

CFrameBuffer* gCurrentBuffer = NULL;

CFrameBuffer::CFrameBuffer(int width, int height)
{
	CL_GraphicContext& gc = g_Display->Graphics().Object();
	
	m_width = width;
	m_height = height;
	m_object = CL_FrameBuffer(gc);
	m_capturing = false;
	m_prevBuffer = NULL;
	m_texture = CL_Texture(gc, width, height);
	m_texture.set_min_filter(cl_filter_nearest);
	m_texture.set_mag_filter(cl_filter_nearest);
	m_object.attach_color_buffer(0, m_texture);
}

CFrameBuffer::CFrameBuffer(CL_FrameBuffer& buffer)
{
	m_width = 0;
	m_height = 0;
	m_object = buffer;
	m_capturing = false;
	m_prevBuffer = NULL;
}

CL_Texture& CFrameBuffer::GetTexture()
{
	return m_texture;
}


void CFrameBuffer::StartNoClear()
{
	if (m_capturing) { return; }

	if (gCurrentBuffer != NULL)
		m_prevBuffer = gCurrentBuffer;

	gCurrentBuffer = this;
	m_capturing = true;

	g_Display->Graphics().Object().set_frame_buffer(m_object);
}

void CFrameBuffer::StartCapture()
{
	if (m_capturing) { return; }

	if (gCurrentBuffer != NULL)
		m_prevBuffer = gCurrentBuffer;

	gCurrentBuffer = this;
	m_capturing = true;

	g_Display->Graphics().Object().set_frame_buffer(m_object);
	g_Display->Graphics().Object().clear(CL_Colorf::black);
	g_Display->Graphics().Object().clear(CL_Colorf::transparent);
}

void CFrameBuffer::StopCapture()
{
	if (m_prevBuffer != NULL && !m_prevBuffer->IsNull())
	{
		g_Display->Graphics().Object().set_frame_buffer(m_prevBuffer->Object());
		gCurrentBuffer = m_prevBuffer;
	}
	else
	{
		g_Display->Graphics().Object().reset_frame_buffer();
		gCurrentBuffer = NULL;
	}

	m_prevBuffer = NULL;
	m_capturing = false;
}

void CFrameBuffer::BindTexture()
{
	g_Display->Graphics().Object().set_texture(0, m_texture);
}

void CFrameBuffer::UnbindTexture()
{
	g_Display->Graphics().Object().reset_texture(0);
}

CL_Texture CFrameBuffer::CopyTexture()
{
	CL_Texture texture = CL_Texture(
		g_Display->Graphics().Object(), m_width, m_height
	);

	texture.set_min_filter(cl_filter_nearest);
	texture.set_mag_filter(cl_filter_nearest);

	CL_PixelBuffer pxBuffer = m_texture.get_pixeldata();
	texture.set_image(pxBuffer);

	return texture;
}

void CFrameBuffer::DrawQuad()
{
	BindTexture();

	CL_GraphicContext& gc = g_Display->Graphics().Object();
	CL_Colorf quadColor = CL_Colorf::white;
	CL_Rectf textureCoords(0.0f, 0.0f, 1.0f, 1.0f);
	CL_Rectf quadRect(
		0, 0, float(gc.get_width()), float(gc.get_height())
	);
	
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
	if (!g_Render->IsShading())
	{
		gc.set_program_object(
			CL_StandardProgram::cl_program_single_texture
		);
	}

	CL_PrimitivesArray primArray(gc);
		primArray.set_attributes(CL_StandardAttributeIndex::cl_attrib_position, quadPositions);
		primArray.set_attributes(CL_StandardAttributeIndex::cl_attrib_texture_position, quadCoords);
		primArray.set_attribute(CL_StandardAttributeIndex::cl_attrib_color, quadColor);
	gc.draw_primitives(cl_triangles, 6, primArray);

	UnbindTexture();
}

void CFrameBuffer::Clear(CL_Colorf& color)
{
		g_Display->Graphics().Object().clear(color);
}

CL_FrameBuffer& CFrameBuffer::Object()
{
	return m_object;
}

bool CFrameBuffer::IsNull()
{
	return m_object.is_null();
}

void CFrameBuffer::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<CFrameBuffer>("FrameBuffer")
		.def(luabind::constructor<int, int>())
		.def("StartNoClear", &CFrameBuffer::StartNoClear)
		.def("StartCapture", &CFrameBuffer::StartCapture)
		.def("StopCapture", &CFrameBuffer::StopCapture)
		.def("UnbindTexture", &CFrameBuffer::UnbindTexture)
		.def("BindTexture", &CFrameBuffer::BindTexture)
		.def("GetTexture", &CFrameBuffer::GetTexture)
		.def("DrawQuad", &CFrameBuffer::DrawQuad)
		.def("CopyTexture", &CFrameBuffer::CopyTexture)
		.def("Clear", &CFrameBuffer::Clear)
		.def("IsNull", &CFrameBuffer::IsNull)
	];
}