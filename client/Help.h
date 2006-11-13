#ifndef XCLONE_HELP_H
#define XCLONE_HELP_H

#include "includes.h"
#include "xcore/GameState.h"
#include "Display.h"
#include "Button.h"
#include "xcore/Game.h"
#include "Audio.h"
#include "FrontEndGameState.h"


class Help : public GameState
{
	public:
	  Help(Game* app);
	  virtual void update(uint32 deltaTime);
      virtual void init();
      virtual void deinit();
      virtual string getName();
      virtual void processSDLEvent(SDL_Event& event);	  
	private:
		Mix_Music* MusicTheme;
		SDL_Surface *background;
		SDL_Surface *message;
		TTF_Font *font;
		//SDL_Color textColor = {117, 117, 117};		
		bool load_files();
};

#endif

