/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CFRAMEBUFFER_H
#define CFRAMEBUFFER_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include "Lua.h"

class CFrameBuffer
{
public:
	CFrameBuffer(CL_FrameBuffer& device);
	CL_Texture& GetTexture();
	CL_Texture CopyTexture();
	void StartNoClear();
	void StartCapture();
	void StopCapture();
	void BindTexture();
	void UnbindTexture();
	void DrawQuad();
	void Clear(CL_Colorf& color);
	CL_FrameBuffer& Object();
	bool IsNull();
	static void LuaBind(luabind::object& globals);
	CFrameBuffer(int width, int height);
private:
	CL_Texture m_texture;
	CFrameBuffer* m_prevBuffer;
	CL_FrameBuffer m_object;
	bool m_capturing;
	int m_width;
	int m_height;
};

#endif