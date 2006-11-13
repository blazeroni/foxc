///The headers
#include "includes.h"
#include "FrontEndGameState.h"
#include "GameLobbyGameState.h"
#include "MainGameState.h"
#include "InventoryState.h"
#include "Help.h"

#define MAIN_THEME "resources/sounds/MainTheme.wav"

FrontEndGameState::FrontEndGameState(Game* app) : 
  GameState(app), joinButton(NULL), hostButton(NULL), helpButton(NULL), exitButton(NULL)
{
	
}

bool FrontEndGameState::load_files()
{
	//display front display method
	background = Display::instance().loadImage("resources/images/background.png", false);
	buttonSheet = Display::instance().loadImage("resources/images/MainMenu.png", false);
	
	MusicTheme = Audio::instance().loadMusic(MAIN_THEME);
	
	if(background == NULL || buttonSheet == NULL)
	    return false;
	
	return true;
}



void FrontEndGameState::init()
{
	load_files();
	Display& ScreenX = Display::instance();
	ScreenX.loadFont("resources/fonts/Floyd.ttf", 72);
	
	Audio::instance().playMusic(-1, MusicTheme);

	SDL_Rect* clips = Button::set_clips();

	hostButton = new Button( (ScreenX.getScreenWidth() - (clips->w))/2, 140, 340, 70, 0, buttonSheet);
	joinButton = new Button( (ScreenX.getScreenWidth() - (clips->w))/2, 210, 340, 70, 2, buttonSheet);
	helpButton = new Button( (ScreenX.getScreenWidth() - (clips->w))/2, 280, 340, 70, 4, buttonSheet);
	exitButton = new Button( (ScreenX.getScreenWidth() - (clips->w))/2, 350, 340, 70, 6, buttonSheet);
	
	
}

string FrontEndGameState::getName()
{
   return "FrontEnd";
}

void FrontEndGameState::processSDLEvent(SDL_Event& event)
{
    hostButton->handle_event(event);           
	joinButton->handle_event(event);           
	helpButton->handle_event(event);           
	exitButton->handle_event(event);           	

	if(event.type == SDL_MOUSEBUTTONDOWN)
	{
		if(event.button.button == SDL_BUTTON_LEFT)
		{
			int x = event.button.x;
			int y = event.button.y;
			//If the mouse is over the button 
			if( ( x > hostButton->getX() ) && ( x < hostButton->getX() + hostButton->getW() ) && ( y > hostButton->getY() ) && ( y < hostButton->getY() + hostButton->getH() ) ) 
			{
/*				Audio::instance().StopMusic();
				GameState* gs = new InventoryState(_game);
				gs->init();
				_game->changeState(gs);		*/		
				return;
			}
			if( ( x > joinButton->getX() ) && ( x < joinButton->getX() + joinButton->getW() ) && ( y > joinButton->getY() ) && ( y < joinButton->getY() + joinButton->getH() ) ) 
			{ 
			    Audio::instance().StopMusic();
				//GameState* gs = new MainGameState(_game);
				//gs->init();
				//_game->changeState(gs);		
            GameState* gs = new GameLobbyGameState(_game);
            gs->init();
            _game->changeState(gs);
				return;
			}
			if( ( x > helpButton->getX() ) && ( x < helpButton->getX() + helpButton->getW() ) && ( y > helpButton->getY() ) && ( y < helpButton->getY() + helpButton->getH() ) ) 
			{ 
			    Audio::instance().StopMusic();
				GameState* gs = new Help(_game);
				gs->init();
				_game->changeState(gs);				
				return;
			}
			if( ( x > exitButton->getX() ) && ( x < exitButton->getX() + exitButton->getW() ) && ( y > exitButton->getY() ) && ( y < exitButton->getY() + exitButton->getH() ) ) 
			{ 
				_game->quit();
			}
		}
	}
}

void FrontEndGameState::update(uint32 X)
{
	Display::instance().draw(0,0, background);
	Display::instance().draw(200,20,"X-CLONE");
	hostButton->show();
	joinButton->show();
	helpButton->show();
	exitButton->show();


}

void FrontEndGameState::deinit()
{
	SAFE_DELETE(hostButton);
	SAFE_DELETE(joinButton);
	SAFE_DELETE(helpButton);
	SAFE_DELETE(exitButton);	
}

