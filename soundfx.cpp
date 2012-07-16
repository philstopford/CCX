// soundfx.c
// Converting to SDL Mixer

#include "main.h"
#include "soundfx.h"
#include "music.h"
#include "resources.h"

// UseSDLMixer now defined in configurations for XCode
// #define UseSDLMixer

#ifdef UseSDLMixer
// SDL
#include "SDL.h"
#include "SDL_mixer.h"
Mix_Chunk* sound[kNumSounds];
#else
// FMod
#include "fmod.h"
#include "fmod_errors.h"
FSOUND_SAMPLE*  sound[kNumSounds];
#endif

MBoolean        soundOn = true;

void InitSound( void )
{
	#ifndef UseSDLMixer
    if (!FSOUND_Init(44100, 64, FSOUND_INIT_USEDEFAULTMIDISYNTH))
	{
		musicOn = soundOn = false; 
		return;
	}
	#endif

	for( int index=0; index<kNumSounds; index++ )
	{
		#ifdef UseSDLMixer
		sound[index] = Mix_LoadWAV( QuickResourceName("snd", index+128, ".wav"));
		#else
		for( index=0; index<kNumSounds; index++ )
		{
			sound[index] = FSOUND_Sample_Load( FSOUND_UNMANAGED, QuickResourceName( "snd", index+128, ".wav" ), FSOUND_NORMAL | FSOUND_LOOP_OFF | FSOUND_2D, 0, 0 );
			if( sound[index] == NULL )
			{
				Error( "InitSound: files are missing" );
			}
		}		
		#endif
	}
}


void PlayMono( short which )
{
	if( soundOn )
	{
		#ifdef UseSDLMixer
		Mix_PlayChannel(-1, sound[which], 0);
		#else
		FSOUND_PlaySound( FSOUND_FREE, sound[which] );
		#endif
	}
}

void PlayStereo( short player, short which )
{
	PlayStereoFrequency( player, which, 0 );
}

void PlayStereoFrequency( short player, short which, short freq )
{
	if( soundOn )
	{
		#ifdef UseSDLMixer
		Mix_PlayChannel(-1, sound[which], 0);
//		Mix_SetPanning(chanHandle, 0, 0);
		#else
		// FMod
		int chanHandle = FSOUND_PlaySoundEx( FSOUND_FREE, sound[which], NULL, true );
		FSOUND_SetPan( chanHandle, player? 255: 0 );
		FSOUND_SetFrequency( chanHandle, (FSOUND_GetFrequency(chanHandle) * (16 + freq)) / 16 );
	    FSOUND_SetPaused( chanHandle, false );
		#endif
	}
}

void UpdateSound()
{
	// no-op!
}
