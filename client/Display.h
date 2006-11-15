#ifndef XCLIENT_DISPLAY_H
#define XCLIENT_DISPLAY_H

#include "includes.h"
#include "xcore/Singleton.h"
#include "Sprite.h"
#include "xcore/MapTile.h"

class Display;
typedef boost::shared_ptr<Display> spDisplay;

class Display : public Singleton<Display>
{
   friend class Singleton<Display>;

   public:
      ~Display();
      void init();
      void deinit();
      SDL_Surface* getScreen() const;
      int getScreenWidth() const;
      int getScreenHeight() const;
      SDL_Surface* loadImage( string filename, bool useAlpha = true );
      bool loadFont(string fileName, int fontSize = 14);
      void draw(int x, int y, string text, bool center = false);
      void draw(int x, int y, SDL_Surface* source, SDL_Rect* clip = NULL);
      void draw(int x, int y, Sprite* s);
      void drawRect(SDL_Rect* rect, SDL_Color* color);
      /// Draw the mouse cursor according to the input mode
      void drawCursor();
      void render();
      /// change the cursor
      void setCursor( SDL_Surface *image = NULL );
      /// draw the most beautiful gui ever made
      void drawGUI(bool showUseButton);
      SDL_Surface *_crosshair;
      /// highlights the usable radius for an item
      void highlightUsable( spMapTile tile, int radius, int offsetx, int offsety, bool ignorelos = false );

   private:
      Display();
      int _screenWidth;
      int _screenHeight;
      SDL_Surface *_screen, *_cursor;
      SDL_Surface *_guiHands, *_guiInv, *_guiUnit, *_guiUse;
      SDL_Surface *_cartridge, *_usableHighlight;
      TTF_Font* _font;

      vector<SDL_Surface*> _loadedImages;
};

#endif
