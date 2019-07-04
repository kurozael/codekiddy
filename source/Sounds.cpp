/*
	© 2011 CloudSixteen.com do not share, re-distribute or modify
	this file without the permission of its owner(s).
*/

#include "Sounds.h"
#include "Util.h"
#include "Files.h"
#include "Game.h"

Sounds::Sounds() : m_output(44100)
{
	CL_SetupSound::init();
	CL_SetupMikMod::init();
	CL_SetupVorbis::init();
}

Sounds::~Sounds()
{
	CL_SetupSound::deinit();
	CL_SetupMikMod::deinit();
	CL_SetupVorbis::deinit();
}

void Sounds::PlaySound(const char* soundPath, float volume)
{
	std::string newSoundPath = Util::FixFilePath(soundPath, "sounds/", ".ogg");
	CL_SoundBuffer sound;

	if (g_Files->IsLocal(newSoundPath.c_str()))
	{
		CL_SoundBuffer sound(newSoundPath, false);
		sound.set_volume(volume);
		sound.play();
	}
	else
	{
		CL_SoundBuffer sound(newSoundPath, false, g_Game->GetVD());
		sound.set_volume(volume);
		sound.play();
	}
}

void Sounds::PlayMusic(const char* soundPath, float volume, bool isLooping, int fadeInTime)
{
	std::string newSoundPath = Util::FixFilePath(soundPath, "sounds/", ".ogg");

	if ( m_music.is_playing() )
	{
		m_music.stop();
	}

	if (volume > 1.0f)
		volume = 1.0f;

	if (g_Files->IsLocal(newSoundPath.c_str()))
	{
		CL_SoundBuffer sound(newSoundPath, false);
		m_music = sound.prepare(isLooping, &m_output);
	}
	else
	{
		CL_SoundBuffer sound(newSoundPath, false, g_Game->GetVD());
		m_music = sound.prepare(isLooping, &m_output);
	}

	CL_FadeFilter fade(0.0f);

	if (fadeInTime > 0)
		m_music.add_filter(fade);

	m_music.set_volume(volume);
	m_music.play();

	if (fadeInTime > 0)
		fade.fade_to_volume(volume, fadeInTime * 1000);
}

void Sounds::FadeMusic(float volume, int fadeTime)
{
	if ( m_music.is_playing() )
	{
		CL_FadeFilter fade(m_music.get_volume());

		if (fadeTime > 0)
			m_music.add_filter(fade);

		fade.fade_to_volume(volume, fadeTime * 1000);
	}
}

void Sounds::ResumeMusic()
{
	if ( !m_music.is_playing() && !m_music.is_null() )
	{
		m_music.play();
	}
}

void Sounds::StopMusic()
{
	if ( m_music.is_playing() )
	{
		m_music.stop();
	}
}

void Sounds::StopAll()
{
	m_output.stop_all();
}

void Sounds::LuaBind(luabind::object& globals)
{
	luabind::module( g_Lua->State() )
	[
		luabind::class_<Sounds>("Sounds")
		.def("ResumeMusic", &Sounds::ResumeMusic)
		.def("PlaySound", &Sounds::PlaySound)
		.def("FadeMusic", &Sounds::FadeMusic)
		.def("PlayMusic", &Sounds::PlayMusic)
		.def("StopMusic", &Sounds::StopMusic)
		.def("StopAll", &Sounds::StopAll)
	];

	globals["SoundsInstance"] = g_Sounds;
}