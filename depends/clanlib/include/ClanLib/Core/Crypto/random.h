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

/// \addtogroup clanCore_Crypto clanCore Crypto
/// \{

#pragma once

#include "../api_core.h"
#include "../System/sharedptr.h"

class CL_DataBuffer;
class CL_Random_Impl;
class CL_Random;

/// \brief Random class
///
/// Uses the operating system cryptographically secure pseudorandom number generator
///
/// \xmlonly !group=Core/Crypto! !header=core.h! \endxmlonly
class CL_API_CORE CL_Random
{
/// \name Construction
/// \{

public:
	/// \brief Constructs the object
	///
	/// cache_size = Random number cache size
	CL_Random(int cache_size = 1024);

/// \}
/// \name Attributes
/// \{

public:

/// \}
/// \name Operations
/// \{

public:
	/// \brief Get the random bytes
	///
	/// out_dest_ptr = Where to write to
	/// num_bytes = Number of bytes to copy
	void get_random_bytes(unsigned char *out_dest_ptr, int num_bytes);

	/// \brief Get the random bytes excluding bytes containing 0
	///
	/// out_dest_ptr = Where to write to
	/// num_bytes = Number of bytes to copy
	void get_random_bytes_nzero(unsigned char *out_dest_ptr, int num_bytes);

	/// \brief Get the random boolean
	///
	/// out_dest_ptr = Where to write to
	/// num_bytes = Number of bytes to copy
	bool get_random_bool();

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_Random_Impl> impl;
/// \}
};

/// \}
