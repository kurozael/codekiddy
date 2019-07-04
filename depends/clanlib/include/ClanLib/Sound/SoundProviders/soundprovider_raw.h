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

/// \addtogroup clanSound_Sound_Providers clanSound Sound Providers
/// \{

#pragma once

#include "../api_sound.h"
#include "soundprovider.h"

class CL_SoundProvider_Raw_Impl;

/// \brief Sound provider in a raw PCM format (no header in file).
///
/// \xmlonly !group=Sound/Sound Providers! !header=sound.h! \endxmlonly
class CL_API_SOUND CL_SoundProvider_Raw : public CL_SoundProvider
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a sound provider based on some raw PCM data.
	///
	/// \param sound_data Raw PCM data.
	/// \param num_samples Number of samples to be read out of sound_data.
	/// \param bytes_per_sample The size of a sample in bytes. This is 2 for 16 bit (signed), and 1 for 8 bit (unsigned).
	/// \param stereo True if sound is stereo (two channels).
	/// \param frequency Playback frequency for sample data.
	CL_SoundProvider_Raw(
		void *sound_data,
		int num_samples,
		int bytes_per_sample,
		bool stereo,
		int frequency = 22050);

	virtual ~CL_SoundProvider_Raw();

/// \}
/// \name Operations
/// \{

public:
	/// \brief Called by CL_SoundBuffer when a new session starts.
	/** \return The soundbuffer session to be attached to the newly started session.*/
	virtual CL_SoundProvider_Session *begin_session();

	/// \brief Called by CL_SoundBuffer when a session has finished. After this call,
	/** <p>CL_SoundBuffer will not access the session anymore. It can safely be deleted
	    here (and in most cases should be delete here).</p>*/
	virtual void end_session(CL_SoundProvider_Session *session);

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_SoundProvider_Raw_Impl> impl;

	friend class CL_SoundProvider_Raw_Session;
/// \}
};

/// \}
