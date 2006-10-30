#ifndef XCLONE_InventoryState_H
#define XCLONE_inventoryState_H

#include "includes.h"
#include "xcore/GameState.h"
#include "Display.h"
#include "Button.h"
#include "xcore/Game.h"
#include "Audio.h"
#include "xcore/Weapon.h"


class InventoryState : public GameState
{
	public:
	  SDL_Rect clips[4];
	  spWeapon WeaponList[4];
	  InventoryState(Game* app);
	  virtual void update(uint32 deltaTime);
      virtual void init();
      virtual void deinit();
      virtual void processSDLEvent(SDL_Event& event);	  
	private:
		Mix_Music* MusicTheme;
		SDL_Surface *soldier;
		SDL_Surface *buttonSheet;
		SDL_Surface *background;
		SDL_Surface *message;
		SDL_Surface *WeaponImage;
		int ImagePosition;
	    SDL_Surface *rightArrow;
        SDL_Surface *leftArrow;
		TTF_Font *font;
		
		Button* ContinueButton;           
		Button* RemoveButton;           
		Button* AddButton;           
		Button* FinishButton;           
		Button* NextButton;
		Button* BackButton;
		//Button* exitButton;           
		bool load_files();
		SDL_Rect* set_clips();
};

#endif

