#ifndef XCLIENT_SPRITE_H
#define XCLIENT_SPRITE_H

#include "includes.h"
#include "Resource.h"

class Sprite : public Resource
{
   public:
      Sprite();
      ~Sprite();

      virtual void load(ticpp::Element* xml);
      SDL_Surface* loadImage(string fileName, bool useAlpha = true);

      void draw(int x, int y);

   private:
      SDL_Surface* _image;
      int _zIndex;
};

#endif
