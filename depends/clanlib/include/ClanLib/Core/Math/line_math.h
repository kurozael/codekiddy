/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Harry Storbacka
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

/// \addtogroup clanCore_Math clanCore Math
/// \{

#pragma once

#include "../api_core.h"
#include "point.h"

/// \brief Math operations on 2D lines.
///
/// Obsolete - Use CL_Line, CL_LineRay, CL_LineSegment
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_API_CORE CL_LineMath
{
public:
// Operations:

	/// \brief Return [<0, 0, >0] if the Point P is right, on or left of the line trough A,B
	///
	/// Also See CL_Line and CL_LineSegment
	///
	/// \param x = x coordinate of the point being tested.
	/// \param y = y coordinate of the point being tested.
	/// \param line = The line. A pointer to a float array with the elements {x1,y1,x2,y2}.
	static float point_right_of_line( float x, float y, float *line );

	/// \brief Point right of line
	///
	/// \param x = value
	/// \param y = value
	/// \param line_x1 = value
	/// \param line_y1 = value
	/// \param line_x2 = value
	/// \param line_y2 = value
	///
	/// \return float
	static float point_right_of_line( float x, float y, float line_x1, float line_y1, float line_x2, float line_y2 );

	/// \brief Point right of line
	///
	/// \param A = Pointf
	/// \param B = Pointf
	/// \param P = Pointf
	///
	/// \return float
	static float point_right_of_line( const CL_Pointf &A, const CL_Pointf &B, const CL_Pointf &P );

	/// \brief Return the midpoint of the line from point A to point B.
	///
	/// Also See CL_LineSegment
	///
	/// \param A = point A
	/// \param B = point B
	/// \return The point
	static CL_Pointf midpoint( const CL_Pointf &A, const CL_Pointf &B );

	/// \brief Return the Point on the line from A to B closest to point P
	///
	/// \param P point P
	/// \param A point A
	/// \param B point B
	static CL_Pointf closest_point( const CL_Pointf &P, const CL_Pointf &A, const CL_Pointf &B );

	/// \brief Return the relative position (0-1) of the point R on the line from A to B closest to point P
	///
	/// \param P point P
	/// \param A point A
	/// \param B point B
	static float closest_point_relative( const CL_Pointf &P, const CL_Pointf &A, const CL_Pointf &B );
};

/// \}
