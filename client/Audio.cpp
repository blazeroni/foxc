#include "includes.h"
#include "Audio.h"
#include "Input.h"
#include "ConfigOptions.h"

#define BLASTER "sounds/blaster.wav"
#define SCREAM "sounds/scream.wav"

Mix_Chunk *Audio::_blaster = NULL;
Mix_Chunk *Audio::_scream = NULL;

Audio::Audio() :
   _music(NULL),
   _effect(NULL)
{
   // empty
}

void Audio::init()
{
   Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);		
   _blaster = loadEffect(BLASTER);
   _scream = loadEffect(SCREAM);
}

void Audio::deinit()
{
   Mix_FreeChunk(_effect);
   Mix_FreeMusic(_music);

   Mix_CloseAudio();
}

Mix_Chunk* Audio::loadEffect(string fileName)
{
   //_effect = Mix_LoadWAV(fileName.c_str());	
   return _effect;
}

Mix_Music* Audio::loadMusic(string fileName)
{
   //_music = Mix_LoadMUS(fileName.c_str());			
   return _music;
}

int Audio::playEffect(Mix_Chunk* effect, int Channel,int numberOfTimes)
{
   if(Mix_PlayChannel(Channel, effect, numberOfTimes) == -1)
      return 1;
   return 0;
}

int Audio::playMusic(int NumOfRep, Mix_Music* music)
{
   if(Mix_PlayingMusic() == 0)
   {
      if(Mix_PlayMusic(music, NumOfRep) == -1)
         return 1;
   } 
   return 0;
}

int Audio::PauseResumeMusic()
{
   if(_music == NULL)
      return 1;
   if(Mix_PausedMusic() == 1)
      Mix_ResumeMusic();
   else
      Mix_PauseMusic();
    return 0;
}

int Audio::StopMusic()
{
   if(_music == NULL)
      return 1;
   Mix_HaltMusic();
   return 0;
}
