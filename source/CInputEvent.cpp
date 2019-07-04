/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "CInputEvent.h"
#include "Display.h"

CInputEvent::CInputEvent() {}

CInputEvent::CInputEvent(CL_InputEvent& input)
{
	m_inputDevice = CInputDevice(input.device);
	m_object = input;
}

CL_Vec2f CInputEvent::GetMousePos()
{
	return CL_Vec2f(m_object.mouse_pos);
}

CInputDevice& CInputEvent::GetInputDevice()
{
	return m_inputDevice;
}

int CInputEvent::GetType()
{
	return m_object.type;
}

int CInputEvent::GetID()
{
	return m_object.id;
}

const char* CInputEvent::GetString()
{
	return m_object.str.c_str();
}

bool CInputEvent::IsAlt()
{
	return m_object.alt;
}

bool CInputEvent::IsShift()
{
	return m_object.shift;
}

bool CInputEvent::IsCtrl()
{
	return m_object.ctrl;
}

void CInputEvent::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<CInputEvent>("InputEvent")
		.def("GetMousePos", &CInputEvent::GetMousePos)
		.def("GetInputDevice", &CInputEvent::GetInputDevice)
		.def("GetType", &CInputEvent::GetType)
		.def("GetString", &CInputEvent::GetString)
		.def("IsAlt", &CInputEvent::IsAlt)
		.def("IsShift", &CInputEvent::IsShift)
		.def("IsCtrl", &CInputEvent::IsCtrl)
		.def("GetID", &CInputEvent::GetID)
	];

	globals["INPUT_POINTER_MOVED"] = CL_InputEvent::pointer_moved;
	globals["INPUT_DOUBLE_CLICK"] = CL_InputEvent::doubleclick;
	globals["INPUT_RELEASED"] = CL_InputEvent::released;
	globals["INPUT_PRESSED"] = CL_InputEvent::pressed;
	globals["INPUT_NO_KEY"] = CL_InputEvent::no_key;
}