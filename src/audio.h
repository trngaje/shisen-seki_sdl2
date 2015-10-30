#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <SDL_mixer.h>

#define AUDIO_SAMPLE_RATE	44100
#define AUDIO_CHUNK_SIZE	1024

extern Mix_Music *bgdMusic;

int initAudio();
void deinitAudio();
Mix_Music *loadMusic(Mix_Music *track, char *fileName);
void unloadMusic(Mix_Music **track);
void playMusic(Mix_Music *track);
void pauseMusic();

#endif /* _AUDIO_H_ */
