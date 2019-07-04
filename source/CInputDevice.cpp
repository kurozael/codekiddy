/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "CInputDevice.h"
#include "Display.h"

CInputDevice::CInputDevice() {}

CInputDevice::CInputDevice(CL_InputDevice& device)
{
	m_object = device;
}

const char* CInputDevice::GetName()
{
	return m_object.get_name().c_str();
}

const char* CInputDevice::GetKeyName(int key)
{
	return m_object.get_key_name(key).c_str();
}

bool CInputDevice::IsKeyDown(int key)
{
	return m_object.get_keycode(key);
}

CL_Vec2f CInputDevice::GetPosition()
{
	return CL_Vec2f( m_object.get_position() );
}

int CInputDevice::GetType()
{
	return m_object.get_type();
}

CL_InputDevice& CInputDevice::Object()
{
	return m_object;
}

bool CInputDevice::IsNull()
{
	return m_object.is_null();
}

void CInputDevice::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<CInputDevice>("InputDevice")
		.def("GetName", &CInputDevice::GetName)
		.def("GetType", &CInputDevice::GetType)
		.def("IsKeyDown", &CInputDevice::IsKeyDown)
		.def("IsNull", &CInputDevice::IsNull)
	];

	globals["DEVICE_KEYBOARD"] = CL_InputDevice::keyboard;
	globals["DEVICE_POINTER"] = CL_InputDevice::pointer;
}