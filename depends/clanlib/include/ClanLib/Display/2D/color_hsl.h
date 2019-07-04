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
**    Mark Page
*/

/// \addtogroup clanDisplay_2D clanDisplay 2D
/// \{

#pragma once

#include "../api_display.h"

class CL_ColorHSLd;
class CL_ColorHSLf;
class CL_Color;
class CL_Colorf;
class CL_Colord;

/// \brief Color HSL description class.
///
/// These templates are defined for: int (CL_ColorHSLi), float (CL_ColorHSLf), double (CL_ColorHSLd)
/// \xmlonly !group=Display/2D! !header=display.h! \endxmlonly
template<typename Type>
class CL_API_DISPLAY CL_ColorHSLx
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a color.
	///
	CL_ColorHSLx() : h((Type) 0), s((Type) 0), l((Type) 0), a((Type) 0) {}
	CL_ColorHSLx(Type h, Type s, Type l, Type a) : h(h), s(s), l(l), a(a) {}
	CL_ColorHSLx(const CL_ColorHSLx<Type> &copy) { h = copy.h; s = copy.s; l = copy.l; a = copy.a;}
	CL_ColorHSLx(const CL_Color &color);
	CL_ColorHSLx(const CL_Colorf &color);
	CL_ColorHSLx(const CL_Colord &color);

/// \}
/// \name Attributes
/// \{
public:
	Type h,s,l,a;

/// \}
/// \name Operations
/// \{

public:

/// \}
/// \name Operators
/// \{
public:
	/// \brief = operator.
	CL_ColorHSLx<Type> &operator = (const CL_ColorHSLx<Type>& copy) { h = copy.h; s = copy.s; l=copy.l; a=copy.a; return *this; }

	/// \brief == operator.
	bool operator == (const CL_ColorHSLx<Type>& color) const {return ((h == color.h) && (s == color.s) && (l == color.l) && (a == color.a));}

	/// \brief != operator.
	bool operator != (const CL_ColorHSLx<Type>& color) const {return ((h != color.h) || (s != color.s) || (l != color.l) || (a != color.a));}

/// \}
};

/// \}

