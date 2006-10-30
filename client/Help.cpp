///The headers
#include "includes.h"
#include "FrontEndGameState.h"
#include "MainGameState.h"
#include "InventoryState.h"
#include "Help.h"

#define MAIN_THEME "resources/sounds/MainTheme.wav"

Help::Help(Game* app) : 
  GameState(app)
{
	
}

bool Help::load_files()
{
	//display front display method
	background = Display::instance().loadImage("resources/images/background.png", false);
	MusicTheme = Audio::instance().loadMusic(MAIN_THEME);
	
	if(background == NULL)
	    return false;
	return true;
}



void Help::init()
{
	load_files();
	Display& ScreenX = Display::instance();
	ScreenX.loadFont("resources/fonts/Floyd.ttf", 20);
	
	Audio::instance().playMusic(-1, MusicTheme);	
}

void Help::processSDLEvent(SDL_Event& event)
{
    if(event.type == SDL_MOUSEBUTTONDOWN)
	{
		if(event.button.button == SDL_BUTTON_LEFT)
		{
			int x = event.button.x;
			int y = event.button.y;
			//If the mouse is over the button 
			Audio::instance().StopMusic();
			GameState* gs = new FrontEndGameState(_game);
			gs->init();
			_game->changeState(gs);				
			return;			
		}
	}
}

void Help::update(uint32 X)
{
	Display::instance().draw(0,0, background);
	Display::instance().draw(20,20,"HELP MENU");
	Display::instance().draw(20,50,"1- To shoot press 'f' and then choose the target");
	Display::instance().draw(20,80,"2- To chat press 'ENTER' and then type your message");
	Display::instance().draw(20,110,"3- to create a new player press 'n'");
	Display::instance().draw(20,170,"TO RETURN JUST CLICK ANYWHERE");
}

void Help::deinit()
{
	
}
