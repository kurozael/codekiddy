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
**    Magnus Norddahl
**    Mark Page
*/

/// \addtogroup clanSound_Filters clanSound Filters
/// \{

#pragma once

#include "../api_sound.h"
#include "../soundfilter.h"

class CL_EchoFilterProvider;

/// \brief Echo Filter Class
///
/// \xmlonly !group=Sound/Filters! !header=sound.h! \endxmlonly
class CL_API_SOUND CL_EchoFilter : public CL_SoundFilter
{
/// \name Construction
/// \{

public:
	/// \brief Echo Filter Constructor
	CL_EchoFilter(int buffer_size = 32*1024, float shift_factor = 2.0f);

	/// \brief Echo Filter Destructor
	virtual ~CL_EchoFilter();

/// \}
/// \name Operations
/// \{

public:

	/// \brief Retrieves the provider.
	CL_EchoFilterProvider *get_provider() const;

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

/// \}
