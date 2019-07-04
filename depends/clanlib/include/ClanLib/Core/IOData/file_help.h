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
**    Kenneth Gangstoe
*/

/// \addtogroup clanCore_I_O_Data clanCore I/O Data
/// \{

#pragma once

#include "../api_core.h"

/// \brief File operation helpers.
///
/// \xmlonly !group=Core/IO Data! !header=core.h! \endxmlonly
class CL_API_CORE CL_FileHelp
{
/// \name Operations
/// \{
public:
	/// \brief Copy a file.
	///
	/// \param from Where to copy from
	/// \param to Where to copy to
	/// \param copy_always true = Copy, even if destination already exists. false = Do not copy if destination exists (throw a CL_Exception)
	static void copy_file(const CL_String &from, const CL_String &to, bool copy_always);

	/// \brief Delete a file.
	///
	/// \param filename File to delete
	static void delete_file(const CL_String &filename);

	/// \brief Check if a file exists.
	///
	/// \param filename File to check for existance
	static bool file_exists(const CL_String &filename);
/// \}
};

/// \}
