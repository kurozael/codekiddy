/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef TIME_H
#define TIME_H

#include "CSingleton.h"
#include "CNonCopyable.h"
#include "Lua.h"

class Time : public CNonCopyable
{
public:
	void Update();
	float CurTime() const;
	float GetDT() const;
	int GetFPS() const;
	void LuaBind(luabind::object& globals);
	~Time();
	Time();
private:
	unsigned int m_lastFrame;
	double m_accumulator;
	float m_deltaTime;
	float m_frameCount;
	float m_curTime;
	int m_currentFPS;
	int m_lastFPS;
};

#define g_Time CSingleton<Time>::Instance()

#endif