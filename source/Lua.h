/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef LUA_H
#define LUA_H

extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
	#include <luajit.h>
}

#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include <luabind/class_info.hpp>
#include <luabind/iterator_policy.hpp>
#include "CNonCopyable.h"
#include "CSingleton.h"

/*
	Don't want to have to do this here, but
	for now it is the best available option.
*/
#include <ClanLib/core.h>
#include <ClanLib/display.h>

class LuaMat4f
{
public:
	CL_Mat4f& Object()
	{
		return m_mat4f;
	}
	void Set(int i, float value)
	{
		m_mat4f.matrix[i] = value;
	}
	float Get(int i)
	{
		return m_mat4f.matrix[i];
	}
	LuaMat4f(LuaMat4f& copy)
	{
		m_mat4f = CL_Mat4f(copy.Object());
	}
	LuaMat4f()
	{
		m_mat4f = CL_Mat4f();
	}
private:
	CL_Mat4f m_mat4f;
};

class Lua : public CNonCopyable
{
public:
	bool FuncExists(const char* funcName);
	void Include(const char* fileName);
	void DoString(const char* string);
	void LuaBind(luabind::object& globals);
	void Print(const char* text);
	void Call(const char* name);
	void Open();
	lua_State* State();
	static Lua* Instance();
	~Lua();
	Lua();
private:
	lua_State* m_state;
	std::string m_fileLoc;
	std::vector<std::string> m_fileStack;
};

#define g_Lua CSingleton<Lua>::Instance()

#endif