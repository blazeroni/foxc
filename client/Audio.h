#ifndef XCLONE_AUDIO_H
#define XCLONE_AUDIO_H

#include "includes.h"
#include "xcore/Singleton.h"
#include <SDL/SDL_mixer.h>

class Audio : public Singleton<Audio>
{

   friend class Singleton<Audio>;
   
   public:
    static Mix_Chunk *_blaster;
    static Mix_Chunk *_scream;

    Audio();
    void init();
    void deinit();
    Mix_Chunk* loadEffect(string fileName);
    Mix_Music* loadMusic(string fileName);
    // luis i changed this so you can play a sound
    // by calling with just the effect name --nick
	// Ok man sounds good!.
    int playEffect(Mix_Chunk* effect, int Channel = 1, int NumOfRep = 0);
    int playMusic(int NumOfRep, Mix_Music* music); // (-1, <name>) makes it keep playing
    int PauseResumeMusic();
    int StopMusic();     

   private:	  
    Mix_Music* _music;
    Mix_Chunk* _effect;
};

#endif
