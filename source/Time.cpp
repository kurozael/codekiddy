/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include <ClanLib/core.h>
#include "Time.h"

Time::Time()
{
	m_lastFrame = 0;
	m_deltaTime = 0.0f;
	m_frameCount = 0.0f;
	m_currentFPS = 0;
	m_lastFPS = 0;
	m_curTime = 0.0f;
}

Time::~Time() {}

float Time::CurTime() const
{
	return m_curTime;
}

int Time::GetFPS() const
{
	return m_lastFPS;
}

void Time::Update()
{
	unsigned int systemTime = CL_System::get_time();
	unsigned int difference = systemTime - m_lastFrame;

	m_deltaTime = (float)difference / 1000.0f;
	m_lastFrame = systemTime;
	m_curTime = (float)systemTime / 1000.0f;

	if (m_deltaTime < 0.01f)
	{
		m_deltaTime = 0.01f;
	}
	else if (m_deltaTime > 0.25f)
	{
		m_deltaTime = 0.25f;
	}

	m_frameCount += m_deltaTime;
	m_currentFPS++;

	if (m_frameCount >= 1.0f)
	{
		m_lastFPS = m_currentFPS;
		m_frameCount = 0;
		m_currentFPS = 0;
	}
}

float Time::GetDT() const
{
	return m_deltaTime;
}

void Time::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<Time>("Time")
		.def("GetDT", &Time::GetDT)
		.def("GetFPS", &Time::GetFPS)
		.def("CurTime", &Time::CurTime)
	];

	globals["TimeInstance"] = g_Time;
}