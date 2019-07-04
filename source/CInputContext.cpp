/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "CInputContext.h"
#include "Display.h"

CInputContext::CInputContext() {}

CInputContext::CInputContext(CL_InputContext& input)
{
		m_object = input;
}

int CInputContext::GetKeyboardCount()
{
	return m_object.get_keyboard_count();
}

int CInputContext::GetMouseCount()
{
	return m_object.get_mouse_count();
}

CInputDevice CInputContext::GetKeyboard()
{
	return CInputDevice( m_object.get_keyboard() );
}

CInputDevice CInputContext::GetMouse()
{
	return CInputDevice( m_object.get_mouse() );
}

CL_InputContext& CInputContext::Object()
{
	return m_object;
}

void CInputContext::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<CInputContext>("InputContext")
		.def("GetKeyboardCount", &CInputContext::GetKeyboardCount)
		.def("GetMouseCount", &CInputContext::GetMouseCount)
		.def("GetKeyboard", &CInputContext::GetKeyboard)
		.def("GetMouse", &CInputContext::GetMouse)
	];
}