/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "CMarchingSquare.h"
#include "Render.h"

std::vector<CL_Vec2f> CMarchingSquare::WalkPerimeter(int sX, int sY)
{
	int textureWidth = m_image.get_width();
	int textureHeight = m_image.get_height();

	if (sX < 0)
		sX = 0;
	if (sX > textureWidth)
		sX = textureWidth;
	if (sY < 0)
		sY = 0;
	if (sY > textureHeight)
		sY = textureHeight;
	
	std::vector<CL_Vec2f> pointList;
	int x = sX;
	int y = sY;
	
	do
	{
		Step(x, y);
	
		if (x >= 0 && x < textureWidth && y >= 0 && y < textureHeight)
			pointList.push_back(CL_Vec2f(float(x), float(y)));
 
		switch (m_nextStep)
		{
			case StepDirection::UP:    y--; break;
			case StepDirection::LEFT:  x--; break;
			case StepDirection::DOWN:  y++; break;
			case StepDirection::RIGHT: x++; break;
			default:
			break;
		}
	} while (x != sX|| y != sY);
 
	return pointList;
}

std::vector<CL_Vec2f> CMarchingSquare::DoMarch()
{
    CL_Vec2f perimeterStart = FindStartPoint();
    return WalkPerimeter((int)perimeterStart.x, (int)perimeterStart.y);
}

bool CMarchingSquare::IsPixelSolid(int x, int y)
{
	int textureWidth = m_image.get_width();

	if (x < 0 || y < 0 || x >= textureWidth || y >= m_image.get_height())
		return false;

	if (m_pixelBuffer.get_pixel(x, y).a > 0)
		return true;
	
	return false;
}

void CMarchingSquare::Step(int x, int y)
{
	bool upLeft = IsPixelSolid(x - 1, y - 1);
	bool upRight = IsPixelSolid(x, y - 1);
	bool downLeft = IsPixelSolid(x - 1, y);
	bool downRight = IsPixelSolid(x, y);

	m_prevStep = m_nextStep;
 
	int state = 0;
 
	if (upLeft)
		state |= 1;
	if (upRight)
		state |= 2;
	if (downLeft)
		state |= 4;
	if (downRight)
		state |= 8;

	switch (state)
	{
		case 1: m_nextStep = StepDirection::UP; break;
		case 2: m_nextStep = StepDirection::RIGHT; break;
		case 3: m_nextStep = StepDirection::RIGHT; break;
		case 4: m_nextStep = StepDirection::LEFT; break;
		case 5: m_nextStep = StepDirection::UP; break;
		case 6:
		if (m_prevStep== StepDirection::UP)
		{
			m_nextStep = StepDirection::LEFT;
		}
		else
		{
			m_nextStep = StepDirection::RIGHT;
		}
		break;
		case 7: m_nextStep = StepDirection::RIGHT; break;
		case 8: m_nextStep = StepDirection::DOWN; break;
		case 9:
		if (m_prevStep== StepDirection::RIGHT)
		{
			m_nextStep = StepDirection::UP;
		}
		else
		{
			m_nextStep = StepDirection::DOWN;
		}
		break;
		case 10: m_nextStep = StepDirection::DOWN; break;
		case 11: m_nextStep = StepDirection::DOWN; break;
		case 12: m_nextStep = StepDirection::LEFT; break;
		case 13: m_nextStep = StepDirection::UP; break;
		case 14: m_nextStep = StepDirection::LEFT; break;
		default:
			m_nextStep = StepDirection::NONE;
			break;
	}
}

CL_Vec2f CMarchingSquare::FindStartPoint()
{
	int textureWidth = m_image.get_width();
	int textureHeight = m_image.get_height();

	for (int x = 0; x < textureWidth; x++)
	{
		for (int y = 0; y < textureHeight; y++)
		{
			if (m_pixelBuffer.get_pixel(x, y).a > 0)
				return CL_Vec2f(float(x), float(y));
		}
	}
	
    return CL_Vec2f(0.0f, 0.0f);
}

CMarchingSquare::CMarchingSquare(const char* fileName)
{
	m_image = g_Render->GetTexture(fileName);
	m_pixelBuffer = m_image.get_pixeldata();
}

CMarchingSquare::~CMarchingSquare() {}