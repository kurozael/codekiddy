/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef CMARCHINGSQUARE_H
#define CMARCHINGSQUARE_H

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <vector>
#include "Lua.h"

class CMarchingSquare
{
private:
	enum StepDirection
	{
		NONE,
		UP,
		LEFT,
		DOWN,
		RIGHT
	};
public:
	std::vector<CL_Vec2f> WalkPerimeter(int sX, int sY);
	std::vector<CL_Vec2f> DoMarch();
	void Step(int x, int y);
	CL_Vec2f FindStartPoint();
	CMarchingSquare(const char* fileName);
	~CMarchingSquare();
private:
	bool IsPixelSolid(int x, int y);
	StepDirection m_prevStep;
	StepDirection m_nextStep;
	CL_PixelBuffer m_pixelBuffer;
	CL_Texture m_image;
};

#endif