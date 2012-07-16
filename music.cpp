#include <string.h>

#include "main.h"
#include "music.h"
#include "gworld.h"
#include "gameticks.h"
#include "soundfx.h"
#include "graphics.h"
#include "resources.h"

const int kNoMusic = -1;

MBoolean musicOn = true, musicFast = false;
int musicLevel = 0, musicSelection = kNoMusic;

// UseSDLMixer now defined in configurations for XCode
// #define UseSDLMixer

#ifdef UseSDLMixer
// SDL Mixer
#include "SDL.h"
#include "SDL_mixer.h"
Mix_Music *musicModule;
#else
#include "fmod.h"
#include "fmod_errors.h"
FMUSIC_MODULE *musicModule = NULL;
#endif

void EnableMusic( MBoolean on )
{
	musicOn = on;
	#ifdef UseSDLMixer
	Mix_VolumeMusic(musicOn? 128: 0);
	#else
	FMUSIC_SetMasterVolume( musicModule, musicOn? 192: 0 );
	#endif
}

void FastMusic( void )
{
	if( musicModule && !musicFast )
	{
		#ifdef UseSDLMixer
		// SDL has no implementation for this, it seems
		musicFast = true;
		#else
		// FMod
		if( musicModule && !musicFast )
		{
			FMUSIC_SetMasterSpeed( musicModule, 1.3f );
			musicFast = true;
		}
		#endif
	}
}

void SlowMusic( void )
{
	if( musicModule && musicFast )
	{
		#ifdef UseSDLMixer
		// SDL has no implementation for this.
		musicFast = false;
		#else
		// FMod
		if( musicModule && musicFast )
		{
			FMUSIC_SetMasterSpeed( musicModule, 1.0f );
			musicFast = false;
		}
		#endif
	}
}

void PauseMusic( void )
{
	if( musicSelection >= 0 && musicSelection <= kSongs )
	{
		#ifdef UseSDLMixer
		Mix_PauseMusic();
		musicLevel++;
		#else
		if( musicSelection >= 0 && musicSelection <= kSongs )
		{
			FMUSIC_SetPaused( musicModule, true );
			musicLevel++;
		}
		#endif
	}
}

void ResumeMusic( void )
{
	if( musicSelection >= 0 && musicSelection <= kSongs )
	{
		#ifdef UseSDLMixer
		musicLevel--;
		Mix_ResumeMusic();
		#else
		if( musicSelection >= 0 && musicSelection <= kSongs )
		{
			musicLevel--;
			FMUSIC_SetPaused( musicModule, false );
		}		
		#endif
	}
}

void ChooseMusic( short which )
{
	if( musicSelection >= 0 && musicSelection <= kSongs )	
	{
		#ifdef UseSDLMixer
		Mix_HaltMusic();
		Mix_FreeMusic(musicModule);
		#else
		// FMod
		FMUSIC_StopSong( musicModule );
		FMUSIC_FreeSong( musicModule );
		#endif
		musicModule = NULL;
	}

	if( which >= 0 && which <= kSongs )
	{
		#ifdef UseSDLMixer
		musicModule = Mix_LoadMUS( QuickResourceName( "mod", which+128, "" ) );
		if (musicModule != NULL)
		{
			Mix_VolumeMusic(musicOn? 128: 0);
			if(Mix_PlayMusic(musicModule, -1)==-1) {
				printf("Mix_PlayMusic: %s\n", Mix_GetError());
			}
			musicSelection = which;
			musicLevel = 0;
		}
		#else
		// FMod
		musicModule = FMUSIC_LoadSong( QuickResourceName( "mod", which+128, "" ) );		
		if( musicModule != NULL )
		{
			FMUSIC_SetMasterVolume( musicModule, musicOn? 192: 0 );
			FMUSIC_SetLooping( musicModule, true );
			FMUSIC_PlaySong( musicModule );
			musicSelection = which;
			musicLevel     = 0;
		}
		#endif
	}
}
