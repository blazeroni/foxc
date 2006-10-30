#ifndef XCLONE_BUTTON_H
#define XCLONE_BUTTON_H

#include "includes.h"

class Button
{
	public:
	Button(int x, int y, int w, int h, int CO, SDL_Surface* ButtonS);
	void handle_event(SDL_Event& event);
	void show();
	int getW();
	int getH();
	int getX();
	int getY();
	static SDL_Rect* set_clips();

	private:
    static SDL_Rect clips[8];
	SDL_Surface *buttonSheet;
	SDL_Rect box;
	SDL_Rect *clip;
	int clipOffset;
	
};

#endif

