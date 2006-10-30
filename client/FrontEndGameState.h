#ifndef XCLONE_FrontEndGameState_H
#define XCLONE_FrontEndGameState_H

#include "includes.h"
#include "xcore/GameState.h"
#include "xcore/Game.h"
#include "Display.h"
#include "Button.h"
#include "Audio.h"


class FrontEndGameState : public GameState
{
	public:
	  FrontEndGameState(Game* game);
	  virtual void update(uint32 deltaTime);
      virtual void init();
      virtual void deinit();
      virtual void processSDLEvent(SDL_Event& event);	  
	private:
		Mix_Music* MusicTheme;
		SDL_Surface *buttonSheet;
		SDL_Surface *background;
		SDL_Surface *message;
		TTF_Font *font;
		//SDL_Color textColor = {117, 117, 117};
		Button* hostButton;           
		Button* joinButton;           
		Button* helpButton;           
		Button* exitButton;           
		bool load_files();
};

#endif

