/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "CDisplayWindow.h"
#include "Files.h"
#include "Game.h"
#include "Util.h"

CDisplayWindow::CDisplayWindow() {}

CDisplayWindow::CDisplayWindow(CL_DisplayWindow& window)
{
	m_object = window;
}

CL_DisplayWindow& CDisplayWindow::Object()
{
	return m_object;
}

void CDisplayWindow::Minimize()
{
	m_object.minimize();
}

void CDisplayWindow::Restore()
{
	m_object.restore();
}

bool CDisplayWindow::IsVisible()
{
	return m_object.is_visible();
}

bool CDisplayWindow::IsMinimized()
{
	return m_object.is_minimized();
}

bool CDisplayWindow::IsMaximized()
{
	return m_object.is_maximized();
}

bool CDisplayWindow::IsFullScreen()
{
	return m_object.is_fullscreen();
}

void CDisplayWindow::SetTitle(const char* title)
{
	m_object.set_title(title);
}

const char* CDisplayWindow::GetTitle()
{
	return m_object.get_title().c_str();
}

CL_Size CDisplayWindow::GetSize()
{
	return m_object.get_gc().get_size();
}

void CDisplayWindow::Flip(int interval)
{
	m_object.flip(interval);
}

bool CDisplayWindow::IsNull()
{
	return m_object.is_null();
}

void CDisplayWindow::SetIcon(const char* materialPath)
{
	std::string newMaterialPath = Util::FixFilePath(materialPath, "materials/", ".png");

	if (g_Files->IsLocal(newMaterialPath.c_str()))
	{
		CL_PixelBuffer icon(newMaterialPath);	

		if ( !icon.is_null() )
		{
			m_object.set_large_icon(icon);
			m_object.set_small_icon(icon);
		}
	}
	else
	{
		CL_PixelBuffer icon(newMaterialPath, g_Game->GetVD());	

		if ( !icon.is_null() )
		{
			m_object.set_large_icon(icon);
			m_object.set_small_icon(icon);
		}
	}
}

void CDisplayWindow::ShowCursor()
{
	m_object.show_cursor();
}

void CDisplayWindow::HideCursor()
{
	m_object.hide_cursor();
}

void CDisplayWindow::SetCursor(const char* materialPath, CL_Origin origin)
{
	std::string newMaterialPath = Util::FixFilePath(materialPath, "materials/", ".png");
	CL_PixelBuffer pixelBuffer;

	if (g_Files->IsLocal(newMaterialPath.c_str()))
		pixelBuffer = CL_PixelBuffer(newMaterialPath);	
	else
		pixelBuffer = CL_PixelBuffer(newMaterialPath, g_Game->GetVD());	

	CL_SpriteDescription spriteDesc;
	spriteDesc.add_frame(pixelBuffer);

	CL_Point hotspot(0, 0);

	if (origin == origin_center)
	{
		hotspot.x = pixelBuffer.get_width() / 2;
		hotspot.y = pixelBuffer.get_height() / 2;
	}

	CL_Cursor cursor(m_object, spriteDesc, hotspot);
	m_object.set_cursor(cursor);
}

void CDisplayWindow::SetCursor(int type)
{
	m_object.set_cursor((CL_StandardCursor)type);
}


void CDisplayWindow::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<CDisplayWindow>("DisplayWindow")
		.def("IsNull", &CDisplayWindow::IsNull)
		.def("SetIcon", &CDisplayWindow::SetIcon)
		.def("GetSize", &CDisplayWindow::GetSize)
		.def("Minimize", &CDisplayWindow::Minimize)
		.def("Restore", &CDisplayWindow::Restore)
		.def("IsVisible", &CDisplayWindow::IsVisible)
		.def("IsMinimized", &CDisplayWindow::IsMinimized)
		.def("IsMaximized", &CDisplayWindow::IsMaximized)
		.def("SetCursorSys", (void(CDisplayWindow::*)(int)) &CDisplayWindow::SetCursor)
		.def("SetCursorImg", (void(CDisplayWindow::*)(const char*, CL_Origin)) &CDisplayWindow::SetCursor)
		.def("HideCursor", &CDisplayWindow::HideCursor)
		.def("ShowCursor", &CDisplayWindow::ShowCursor)
		.def("SetTitle", &CDisplayWindow::SetTitle)
		.def("GetTitle", &CDisplayWindow::GetTitle)
		.def("IsFullScreen", &CDisplayWindow::IsFullScreen)
	];
}