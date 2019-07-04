/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "wtypes.h"
#include "Display.h"
#include "CGraphicContext.h"
#include "CDisplayWindow.h"

Display::~Display() {}

Display::Display() : m_newWidth(0), m_newHeight(0) {}

CDisplayWindow& Display::Window()
{
	return m_window;
}

int Display::GetH()
{
	return m_window.GetSize().height;
}

int Display::GetW()
{
	return m_window.GetSize().width;
}

bool Display::Create(unsigned int width, unsigned int height, const char* title)
{
	CL_DisplayWindowDescription windowDesc;
		windowDesc.set_title(title);
		windowDesc.set_size(CL_Size(width, height), true);
		windowDesc.set_allow_resize(false);
		windowDesc.show_maximize_button(false);
	CL_DisplayWindow window = CL_DisplayWindow(windowDesc);

	m_window = CDisplayWindow(window);
	m_window.SetIcon("icon.png");
	m_graphics = CGraphicContext( m_window.Object().get_gc() );
	m_input = CInputContext( m_window.Object().get_ic() );
	
	return true;
}

void Display::SetSize(unsigned int width, unsigned int height, bool bRightNow)
{
	if (!bRightNow)
	{
		m_newWidth = width;
		m_newHeight = height;
	}
	else
	{
		m_window.Object().set_size(width, height, true);
	}
}

CL_Mat4f Display::GetViewMatrix()
{
	CL_Mat4f mat4f = m_graphics.Object().get_modelview();
	return mat4f;
}

void Display::Update()
{
	if (m_newWidth != 0 || m_newHeight != 0)
	{
		m_window.Object().set_size(m_newWidth, m_newHeight, true);
			g_Lua->Call("OnWindowResize");
		m_newWidth = 0; m_newHeight = 0;
	}
}

void Display::Print(const char* text)
{
	CL_Console::write_line(text);
}

void Display::Clear(CL_Colorf &color)
{
	m_graphics.Clear(color);
}

void Display::Flip(int interval)
{
	m_window.Flip(interval);
	m_graphics.Reset();
}

CGraphicContext& Display::Graphics()
{
	return m_graphics;
}

CInputContext& Display::Inputs()
{
	return m_input;
}

/* This is some wizardry (may need to remove * 255) */
float Display::ToFloat(const CL_Colorf& color)
{
	int fColor = ((int)(255 * (color.a * 255)) << 24)
		| ((int)(255 * (color.b * 255)) << 16)
		| ((int)(255 * (color.g * 255)) << 8)
		| ((int)(255 * (color.r * 255)));

	union {float f; int i;} u;
		u.i = fColor;
	return u.f;
}

void Display::SetWindowPos(int x, int y)
{
	m_window.Object().set_position(x, y);
}

CL_Sizef Display::GetDesktopSize()
{
	RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	return CL_Sizef(float(desktop.right), float(desktop.bottom));
}

void Display::CenterWindow()
{
	CL_Sizef desktopSize = GetDesktopSize();

	SetWindowPos(
		int((desktopSize.width / 2) - (GetW() / 2)),
		int((desktopSize.height / 2) - (GetH() / 2))
	);
}

void Display::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<Display>("Display")
		.def("GetViewMatrix", &Display::GetViewMatrix)
		.def("SetWindowPos", &Display::SetWindowPos)
		.def("CenterWindow", &Display::CenterWindow)
		.def("GetDesktopSize", &Display::GetDesktopSize)
		.def("SetSize", &Display::SetSize)
		.def("Window", &Display::Window)
		.def("Graphics", &Display::Graphics)
		.def("ToFloat", &Display::ToFloat)
		.def("Print", &Display::Print)
		.def("Inputs", &Display::Inputs)
		.def("Clear", &Display::Clear)
		.def("GetH", &Display::GetH)
		.def("GetW", &Display::GetW)
	];

	globals["DisplayInstance"] = g_Display;
}