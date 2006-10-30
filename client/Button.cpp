#include "includes.h"
#include "Button.h"
#include "xcore/GameState.h"
#include "Display.h"

SDL_Rect Button::clips[8];

int Button::getH() { return box.h; }
int Button::getW() { return box.w; }
int Button::getY() { return box.y; }
int Button::getX() { return box.x; }

Button::Button(int x, int y, int w, int h, int CO, SDL_Surface* ButtonS)
{
    box.x = x;
	box.y = y;
	box.h = h;
	box.w = w;
	buttonSheet = ButtonS;
	clipOffset = CO;
	clip = &clips[0 + CO];			
}

SDL_Rect* Button::set_clips()
{
    //Clip the sprite sheet //W340, H70
    clips[0].x = 0;
    clips[0].y = 0;
    clips[0].w = 340;
    clips[0].h = 70;

    clips[1].x = 340;
    clips[1].y = 0;
    clips[1].w = 340;
    clips[1].h = 70;

	clips[2].x = 0;
    clips[2].y = 70;
    clips[2].w = 340;
    clips[2].h = 70;

    clips[3].x = 340;
    clips[3].y = 70;
    clips[3].w = 340;
    clips[3].h = 70;

	clips[4].x = 0;
    clips[4].y = 140;
    clips[4].w = 340;
    clips[4].h = 70;

    clips[5].x = 340;
    clips[5].y = 140;
    clips[5].w = 340;
    clips[5].h = 70;

	clips[6].x = 0;
    clips[6].y = 210;
    clips[6].w = 340;
    clips[6].h = 70;

    clips[7].x = 340;
    clips[7].y = 210;
    clips[7].w = 340;
    clips[7].h = 70;

	return clips;
}

void Button::handle_event(SDL_Event& event)
{
	int x = 0, y = 0;
	// If mouse moves
	if(event.type == SDL_MOUSEMOTION)
	{
		x = event.button.x;
		y = event.button.y;

		// if the mouse is over the button
		if((x > box.x) && (x < box.x + box.w) && (y > box.y) && (y < box.y + box.h))
		    clip = &clips[1 + clipOffset];
		else
			clip = &clips[0 + clipOffset];
	}
}

void Button::show()
{
	Display::instance().draw(box.x, box.y, buttonSheet, clip);	
}

void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip = NULL)
{
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;

	//blit
	SDL_BlitSurface(source, clip, destination, &offset);
}

/*
SDL_Surface *load_image( std::string filename ) 
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    
    loadedImage = IMG_Load( filename.c_str() );
    
    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormat( loadedImage );
        SDL_FreeSurface( loadedImage );
    }
    if(optimizedImage != NULL)
	{
		//MAP the Color Key
		Uint32 colorkey = SDL_MapRGB(optimizedImage->format, 0xFF, 0xFF, 0xFF); 
		//Set all pixels of color (255,255,255) tp be transparent
		SDL_SetColorKey(optimizedImage, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey);
	}	
    return optimizedImage;
}*/

