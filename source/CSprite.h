/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CSPRITE_H
#define CSPRITE_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include "CBoundingBox.h"
#include "CEventHandler.h"
#include "Lua.h"

class CSprite : public CEventHandler
{
public:
	CSprite();
	CSprite(const char* fileName);
	virtual ~CSprite();
	int GetH() const;
	int GetW() const;
	void StartFrames();
	void AddSheet(const char* fileName, int x, int y, int w, int h, int framesX, int framesY, int spacingX, int spacingY, int skipFrames = 0);
	void AddImage(const char* fileName);
	void FinishFrames();
	void SetDelay(float seconds);
	void CopyFrames(CSprite& sprite);
	void CopyModes(CSprite& sprite);
	void SetLooping(bool isLooping);
	void SetRotateSpot(CL_Origin origin);
	void SetTranslateSpot(CL_Origin origin);
	bool GetLooping();
	void SetPingPong(bool isPingPong);
	bool GetPingPong();
	CL_Sprite& Object();
	static void LuaBind(luabind::object& globals);
	virtual void Update();
	virtual void Draw(float x, float y, float w, float h, CL_Angle& angle, CL_Colorf& color);
	virtual void DrawAroundPoint(float x, float y, float w, float h, CL_Angle& angle, CL_Colorf& color, CL_Vec2f& point);
protected:
	CL_SpriteDescription m_spriteDesc;
	CL_Sprite m_sprite;
	const char* m_image;
	bool m_addingFrames;
};

#endif