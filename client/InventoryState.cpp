///The headers
#include "includes.h"
#include "InventoryState.h"
#include "MainGameState.h"
#include "FrontEndGameState.h"

#define INVEN_THEME "resources/sounds/MainTheme.wav"

InventoryState::InventoryState(Game* app) : 
  GameState(app), ContinueButton(NULL), RemoveButton(NULL), AddButton(NULL), NextButton(NULL), BackButton(NULL), FinishButton(NULL)
{
	ImagePosition = 0;
}

bool InventoryState::load_files()
{
	//display front display method
	background = Display::instance().loadImage("resources/images/background.png", false);
	buttonSheet = Display::instance().loadImage("resources/images/InventorySheet.png", false);
	rightArrow = Display::instance().loadImage("resources/images/arrow-right.gif", false);
	leftArrow = Display::instance().loadImage("resources/images/arrow-left.gif", false);
	soldier = Display::instance().loadImage("resources/images/soldier2.gif", false);	
	MusicTheme = Audio::instance().loadMusic(INVEN_THEME);
	
/*	
   WeaponList[0] = spWeapon( new Pistol() );
    WeaponList[1] = spWeapon( new Rifle() );
	WeaponList[2] = spWeapon( new Bazooka() );
	WeaponList[3] = spWeapon( new Grenade() );
*/
	//WeaponImage = WeaponList[0]->getImage();	
	
	if(background == NULL)
	    return false;
	
	return true;
}



void InventoryState::init()
{
	load_files();
	Display& ScreenX = Display::instance();
	ScreenX.loadFont("resources/fonts/Floyd.ttf", 50);
	
	Audio::instance().playMusic(-1, MusicTheme);		

	SDL_Rect* clip = InventoryState::set_clips();

	ContinueButton = new Button( 0, 310, 340, 70, 0, buttonSheet);
	RemoveButton = new Button( 0, 380, 340, 70, 2, buttonSheet);
	AddButton = new Button( 0, 450, 340, 70, 4, buttonSheet);
	FinishButton = new Button( 0, 520, 340, 70, 6, buttonSheet);

	NextButton = new Button( 250, 100, 50, 50, 0, rightArrow);
	BackButton = new Button( 0, 100, 50, 50, 0, leftArrow);

}

SDL_Rect* InventoryState::set_clips()
{
    //Clip the sprite sheet //W340, H70
    clips[0].x = 0;
    clips[0].y = 0;
    clips[0].w = 100;
    clips[0].h = 80;

    clips[1].x = 100;
    clips[1].y = 0;
    clips[1].w = 100;
    clips[1].h = 80;

	clips[2].x = 0;
    clips[2].y = 80;
    clips[2].w = 100;
    clips[2].h = 80;

    clips[3].x = 100;
    clips[3].y = 80;
    clips[3].w = 100;
    clips[3].h = 80;	

	return clips;
}

void InventoryState::processSDLEvent(SDL_Event& event)
{
    ContinueButton->handle_event(event);           
	RemoveButton->handle_event(event);           
	AddButton->handle_event(event);
	FinishButton->handle_event(event);
	BackButton->handle_event(event);
	NextButton->handle_event(event);

	if(event.type == SDL_MOUSEBUTTONDOWN)
	{
		if(event.button.button == SDL_BUTTON_LEFT)
		{
		    int x = event.button.x;
			int y = event.button.y;
			//If the mouse is over the button 
			if( ( x > ContinueButton->getX() ) && ( x < ContinueButton->getX() + ContinueButton->getW() ) && ( y > ContinueButton->getY() ) && ( y < ContinueButton->getY() + ContinueButton->getH() ) ) 
			{
				Audio::instance().StopMusic();
				GameState* gs = new MainGameState(_game);
				gs->init();
				_game->changeState(gs);								
				return;
			}
			if( ( x > NextButton->getX() ) && ( x < NextButton->getX() + NextButton->getW() ) && ( y > NextButton->getY() ) && ( y < NextButton->getY() + NextButton->getH() ) ) 
			{
//				WeaponImage = WeaponList[ImagePosition++ % 4]->getImage();
				return;
			}
			if( ( x > BackButton->getX() ) && ( x < BackButton->getX() + BackButton->getW() ) && ( y > BackButton->getY() ) && ( y < BackButton->getY() + BackButton->getH() ) ) 
			{
				if(ImagePosition > 0)
            {
//				    WeaponImage = WeaponList[--ImagePosition % 4]->getImage();
            }
				else
				{
					ImagePosition = 4;
//					WeaponImage = WeaponList[--ImagePosition % 4]->getImage();
				}
				return;
			}
			if( ( x > AddButton->getX() ) && ( x < AddButton->getX() + AddButton->getW() ) && ( y > AddButton->getY() ) && ( y < AddButton->getY() + AddButton->getH() ) ) 
			{
				// IN HERE IT SHOULD ADD THE CHOSEN WEAPON TO THE UNIT				
			}
		}
	}
}

void InventoryState::update(uint32 X)
{
	Display::instance().draw(0,0, background);
	Display::instance().draw(500,280, soldier);
	//Display::instance().draw(70,100, WeaponImage);
	Display::instance().draw(20,20,"INVENTORY SELECTION");
	
	ContinueButton->show();
	RemoveButton->show();
	AddButton->show();
	FinishButton->show();
	BackButton->show();
	NextButton->show();

}

void InventoryState::deinit()
{
    SAFE_DELETE(ContinueButton);
	SAFE_DELETE(AddButton);
	SAFE_DELETE(RemoveButton);
}

