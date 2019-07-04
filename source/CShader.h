/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CSHADER_H
#define CSHADER_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "Lua.h"
#include <map>

class CShader
{
public:
	CShader(CL_ProgramObject& device);
	void SetTexture(const char* key, CL_Texture& texture, int id);
	void SetFloat(const char* key, float value);
	void SetColor(const char* key, CL_Colorf& color);
	void SetVec2(const char* key, CL_Vec2f& vec2);
	void SetMat4(const char* key, LuaMat4f& mat4);
	void SetInt(const char* key, int value);
	void Bind();
	void Unbind();
	bool IsNull();
	CL_ProgramObject& Object();
	static void LuaBind(luabind::object& globals);
	CShader(const char* vertex, const char* fragment);
private:
	CL_ProgramObject m_object;
	std::map<int, CL_Texture> m_idMap;
};

#endif