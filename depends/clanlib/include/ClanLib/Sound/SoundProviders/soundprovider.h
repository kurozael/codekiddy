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
*/

/// \addtogroup clanSound_Audio_Mixing clanSound Audio Mixing
/// \{

#pragma once

#include "../api_sound.h"
#include "../../Core/System/sharedptr.h"

class CL_SoundProvider_Session;
class CL_SoundProvider_Impl;

/// \brief Sound provider.
///
/// Interface to sound sources. Used by the CL_SoundBuffer class to start and stop playback.
/// \xmlonly !group=Sound/Audio Mixing! !header=sound.h! \endxmlonly
class CL_API_SOUND CL_SoundProvider
{
/// \name Construction
/// \{

public:
	CL_SoundProvider();

	virtual ~CL_SoundProvider();

/// \}
/// \name Operations
/// \{

public:
	/// \brief Called by CL_SoundBuffer when a new session starts.
	/** \return The soundbuffer session to be attached to the newly started session.*/
	virtual CL_SoundProvider_Session *begin_session() = 0;

	/// \brief Called by CL_SoundBuffer when a session has finished. After this call,
	/** <p>CL_SoundBuffer will not access the session anymore. It can safely be deleted
	    here (and in most cases should be delete here).</p>*/
	virtual void end_session(CL_SoundProvider_Session *session) = 0;

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_SoundProvider_Impl> impl;
/// \}
};

/// \}
