/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#ifndef SOUNDS_H
#define SOUNDS_H

#include <ClanLib/core.h>
#include <ClanLib/sound.h>
#include <ClanLib/vorbis.h>
#include <ClanLib/mikmod.h>
#include "CNonCopyable.h"
#include "CSingleton.h"
#include "Lua.h"
#include <map>

class Sounds : public CNonCopyable
{
public:
	void PlaySound(const char* fileName, float volume = 1.0f);
	void PlayMusic(const char* fileName, float volume, bool isLooping = false, int fadeInTime = 0);
	void FadeMusic(float volume, int fadeTime);
	void ResumeMusic();
	void StopMusic();
	void StopAll();
	void LuaBind(luabind::object& globals);
	~Sounds();
	Sounds();
protected:
	CL_SoundBuffer_Session m_music;
	CL_SoundOutput m_output;
	std::map<const char*, CL_SoundBuffer> m_id;
};

#define g_Sounds CSingleton<Sounds>::Instance()

#endif