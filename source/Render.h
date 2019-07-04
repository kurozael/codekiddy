/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef RENDER_H
#define RENDER_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "CFrameBuffer.h"
#include "CShader.h"
#include "Files.h"
#include "Lua.h"

class Render : public CNonCopyable
{
public:
	void OverrideColor(CL_Colorf& color = CL_Colorf::transparent);
	void DrawPixel(float x, float y, CL_Colorf& color);
	void DrawLine(float x1, float y1, float x2, float y2, CL_Colorf& color);
	void DrawBox(float x, float y, float w, float h, CL_Colorf& color);
	void DrawFill(float x, float y, float w, float h, CL_Colorf& color);
	void DrawTexture(CL_Texture& texture, float x, float y, float w, float h, CL_Colorf& color);
	void DrawImage(CL_Image& image, float x, float y, float w, float h, CL_Colorf& color);
	void SetBlendFunc(int source, int destination);
	void SetBlendColor(CL_Colorf& color);
	bool IsShading();
	void SetShading(bool bIsShading);
	void StartBlending();
	void StopBlending();
	void SetDepthMask(bool bDepthMask);
	bool IsBlending();
	void DrawGradientFill(float x, float y, float w, float h, CL_Gradient& gradient);
	void DrawCircle(float x, float y, float radius, CL_Colorf& color);
	void DrawGradientCircle(float x, float y, float radius, CL_Gradient& gradient);
	CL_Image GetImage(const char* fileName);
	CL_Texture GetTexture(const char* fileName);
	void LuaBind(luabind::object& globals);
	~Render();
	Render();
private:
	CL_BlendMode m_blendDefault;
	CL_BlendMode m_blendMode;
	CL_Colorf m_color;
	bool m_blending;
	bool m_shading;
};

#define g_Render CSingleton<Render>::Instance()

#endif