#include "includes.h"
#include "Display.h"
#include "Input.h"
#include "ConfigOptions.h"
#include "MainGameState.h"
#include "ClientUnit.h"
#include "XClient.h"

#define CROSSHAIR_IMAGE "resources/images/crosshair-white.png"

Display::Display() :
   _screen(NULL),
   _font(NULL)
{
   // empty
}

Display::~Display()
{
   vector<SDL_Surface*>::iterator iter;
   for (iter = _loadedImages.begin(); iter != _loadedImages.end(); ++iter)
   {
      SAFE_DELETE(*iter);
   }
}

void Display::init()
{
   ConfigOptions& o = ConfigOptions::instance();

   _screenWidth = o.get<int>(WIDTH);
   _screenHeight = o.get<int>(HEIGHT);

   _screen = SDL_SetVideoMode(_screenWidth,
                              _screenHeight,
                              o.get<int>(COLOR_DEPTH),
                              SDL_HWSURFACE | SDL_DOUBLEBUF);
   SDL_WM_SetCaption("X-CLONE", NULL);
   _crosshair = loadImage( CROSSHAIR_IMAGE );
   _guiHands = loadImage( "resources/images/gui/br.png" );
   _guiInv = loadImage( "resources/images/gui/inventory_gui.png" );
   _guiUnit = loadImage( "resources/images/gui/unit_gui.png" );
   _guiUse = loadImage( "resources/images/gui/use_object.png" );
   _cartridge = loadImage( "resources/images/gui/cartridge_gui.png" );
   _usableHighlight = loadImage( "resources/images/tile_usable.png" );
   _cursor = NULL;
}

void Display::deinit()
{
   if (_screen)
   {
      SDL_FreeSurface(_screen);
   }

   if (_font)
   {
      TTF_CloseFont(_font);
   }
}

SDL_Surface* Display::getScreen() const
{
   return _screen;
}

int Display::getScreenWidth() const
{
   return _screenWidth;
}

int Display::getScreenHeight() const
{
   return _screenHeight;
}

SDL_Surface* Display::loadImage( string filename, bool useAlpha ) 
{
   SDL_Surface* loadedImage = NULL;
   SDL_Surface* optimizedImage = NULL;
   //Load the image using SDL_image
   cout << filename << endl;
   loadedImage = IMG_Load( filename.c_str() );

   if(loadedImage != NULL)
   {
      //Create an optimized image
      if (useAlpha)
      {
         optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
      }
      else 
      {
         optimizedImage = SDL_DisplayFormat( loadedImage );
      }
      SDL_FreeSurface( loadedImage );
   }
   else
   {
      cout << SDL_GetError() << endl;
   }
    
   if(optimizedImage != NULL && !useAlpha)
   {
      //Map the color key
      Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0xFF, 0, 0xFF );          
      SDL_SetColorKey( optimizedImage, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey );
   }
   
   if (optimizedImage == NULL)
   {
      cout << SDL_GetError() << endl;
   }

   _loadedImages.push_back(optimizedImage);

   return optimizedImage;
}

void Display::draw(int x, int y, string text, bool center)
{
   if (_font)
   {
      SDL_Color color = {255, 255, 255};
      SDL_Surface* message = TTF_RenderText_Solid(_font, text.c_str(), color);

      if (message)
      {
        if ( center )
            draw(x - message->w/2, y-message->h/2, message);
        else
            draw(x, y, message);
         SDL_FreeSurface(message);
      }
   }
}

void Display::draw(int x, int y, SDL_Surface* source, SDL_Rect* clip)
{
   //Rectangle to hold the offsets
   SDL_Rect offset;
   //Get offsets
   offset.x = x;
   offset.y = y;
   //Blit the surface
   SDL_BlitSurface( source, clip, _screen, &offset );
}

void Display::drawRect(SDL_Rect* rect, SDL_Color* color)
{
    SDL_FillRect( _screen, rect, SDL_MapRGB(_screen->format, color->r, color->g, color->b) );
}

void Display::render()
{
   SDL_Flip(_screen);
   SDL_FillRect(_screen, NULL, 0);
}

bool Display::loadFont(string fileName, int fontSize)
{
   _font = TTF_OpenFont(fileName.c_str(), fontSize);
   if (_font)
   {
      return true;
   }

   return false;
}

void Display::drawCursor()
{
    // crosshair for fire mode
    //if ( Input::instance().getMode() == FIRE )
    //{
    if ( !_cursor )
        return;
    Point mousePosition = Input::instance().getMousePosition();
    draw( mousePosition.x-(_cursor->w/2), mousePosition.y-(_cursor->h/2), _cursor );
    //}
}

void Display::drawGUI(bool showUseButton)
{
    MainGameState *gs = (MainGameState*)XClient::instance().getCurrentGameState();
    spUnit activeUnit = gs->getActiveUnit();
    spUnit selectedUnit = ClientUnit::getSelectedUnit();

    if ( activeUnit.get() )
    {
        // Unit
        draw( _screenWidth - _guiUnit->w, 0, _guiUnit );
        draw( _screenWidth - 155 - ClientUnit::_image->w/2, 40 - ClientUnit::_image->h/2, ClientUnit::_image ); 
        draw( _screenWidth - 65, 20, activeUnit->getName(), true );
        stringstream ss;
        string text = "";
        if ( gs->getLocalPlayer()->getID() == activeUnit->getPlayerID() )
        {
            ss << max ( activeUnit->getHealth(), 0 );
            ss >> text;
        }
        else
            text = "?";
        text = "HP:"+text;
        draw( _screenWidth - 90, 45, text, true );
        ss.clear(stringstream::goodbit);
        text = "";
        if ( gs->getLocalPlayer()->getID() == activeUnit->getPlayerID() )
        {
            ss << max ( activeUnit->getActionPoints(), 0 );
            ss >> text;
        }
        else
            text = "?";
        text = "AP:"+text;
        draw( _screenWidth - 35, 45, text, true );
        if ( selectedUnit.get() && selectedUnit != activeUnit )
        {
            draw( _screenWidth - _guiUnit->w, _guiUnit->h - 5, _guiUnit );
            draw( _screenWidth - 155 - ClientUnit::_image->w/2, _guiUnit->h - 5 + 40 - ClientUnit::_image->h/2, ClientUnit::_image ); 
            draw( _screenWidth - 65, _guiUnit->h - 5+ 20, selectedUnit->getName(), true );
            ss.clear(stringstream::goodbit);
            text = "";
            if ( gs->getLocalPlayer()->getID() == selectedUnit->getPlayerID() )
            {
               ss << max ( activeUnit->getHealth(), 0 );
               ss >> text;
            }
            else
				   text = "?";
            text = "HP:"+text;
            draw( _screenWidth - 90, _guiUnit->h - 5+ 45, text, true );
            ss.clear(stringstream::goodbit);
            text = "";
            if ( gs->getLocalPlayer()->getID() == selectedUnit->getPlayerID() )
            {
               ss << max ( activeUnit->getActionPoints(), 0 );
               ss >> text;
            }
            else
               text = "?";
            text = "AP:"+text;
            draw( _screenWidth - 35, _guiUnit->h - 5+ 45, text, true );
        }

        if ( !gs->isMyTurn() )
            return;
    
        // Use Button
        if (showUseButton)
        {
           draw(_screenWidth - _guiUse->w, _screenHeight - _guiUse->h - _guiHands->h, _guiUse);
           draw(_screenWidth - _guiUse->w / 2, _screenHeight - _guiUse->h / 2 - _guiHands->h, "Use Door", true);
        }

        //Hands
        draw( _screenWidth - _guiHands->w, _screenHeight - _guiHands->h, _guiHands );
        spItem item1 = spItem(), item2 = spItem();
        item1 = activeUnit->getLeft();
        item2 = activeUnit->getRight();
        if ( item1.get() )
        {
            draw( _screenWidth - 145 - item1->getImage()->w/2, _screenHeight - 70 - item1->getImage()->h/2, item1->getImage());
            draw( _screenWidth - 145, _screenHeight - 145, item1->getName(), true );
        for ( int i = 1; i <= 12; ++i )
        {
            if ( item1->hasAmmo() >= i )
                draw( _screenWidth - 95 - _cartridge->w/2, _screenHeight - 30 - (i-1)*_cartridge->h, _cartridge );
        }
        }
        if ( item2.get() )
        {
            draw( _screenWidth - 45 - item2->getImage()->w/2, _screenHeight - 70 - item2->getImage()->h/2, item2->getImage());
            draw( _screenWidth - 45, _screenHeight - 145, item2->getName(), true );
        }

        //Inventory
        draw( _screenWidth - _guiHands->w - _guiInv->w + 5, _screenHeight - _guiInv->h, _guiInv );
        spItem invItem;
        for ( int i = 0; i < 5; ++i )
        {
            invItem = activeUnit->getInv(i);
            if ( invItem.get() )
            {
                draw( _screenWidth - _guiHands->w - 10 - 17 - 65*i - invItem->getInvImage()->w/2, _screenHeight - 37 - invItem->getInvImage()->h/2, invItem->getInvImage() );
            }
        }
    }
}

void Display::setCursor( SDL_Surface *image )
{
    _cursor = image;
    SDL_ShowCursor( SDL_DISABLE );
    if ( !image ) 
        SDL_ShowCursor( SDL_ENABLE );
}

void Display::highlightUsable( spMapTile tile, int radius, int ox, int oy )
{
    MainGameState* gs = (MainGameState*)XClient::instance().getCurrentGameState();
    spMap map = gs->getMap();
    spMapTile tile2;
    for ( int i = 0; i < map->getWidth(); ++i )
    {
        for ( int j = 0; j < map->getHeight(); ++j )
        {
            tile2 = map->getTile(i,j);
            if ( tile->getDistance(tile2) <= radius )
                draw( tile2->getScreenX()-ox, tile2->getScreenY()-oy, _usableHighlight );
        }
    }
}

