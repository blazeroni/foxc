#include "includes.h"
#include "Sprite.h"

Sprite::Sprite() :
   _image(NULL),
   _zIndex(0)
{
}

Sprite::~Sprite()
{
}

void Sprite::load(ticpp::Element* xml)
{
   string filename;
   bool useAlpha;
   xml->GetAttribute("filename", &filename);
   xml->GetAttributeOrDefault("useAlpha", &useAlpha, true);

   _image = loadImage("resources/" + filename, useAlpha);
}

SDL_Surface* Sprite::loadImage( string filename, bool useAlpha ) 
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
   return optimizedImage;
    
   if(optimizedImage != NULL)
   {
      //Map the color key
      Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0xFF, 0, 0xFF );          
      SDL_SetColorKey( optimizedImage, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey );
   }
   else
   {
      cout << SDL_GetError() << endl;
   }

   return optimizedImage;
}
