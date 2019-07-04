/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef LIGHTS_H
#define LIGHTS_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "CNonCopyable.h"
#include "CSingleton.h"
#include "CLight.h"
#include <map>
#include "Lua.h"

typedef std::map<std::string, CLight> LightMap;

class Lights : public CNonCopyable
{
public:
	CLight& Add(std::string uniqueID, CL_Colorf& color, CL_Vec2f& position, float w, float h, float intensity);
	void SetAmbience(CL_Colorf& ambience);
	CL_Colorf& GetAmbience();
	void Remove(std::string uniqueID);
	void Finish();
	void Begin();
	void Init();
	void LuaBind(luabind::object& globals);
	~Lights();
	Lights();
protected:
	CL_FrameBuffer m_frameBuffer;
	CL_Texture m_lightTexture;
	CL_Image m_lightImage;
	CL_Texture m_lightMask;
	CL_Colorf m_ambience;
	LightMap m_lightMap;
};

#define g_Lights CSingleton<Lights>::Instance()

#endif