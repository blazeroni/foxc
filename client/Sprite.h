#ifndef XCLIENT_SPRITE_H
#define XCLIENT_SPRITE_H

#include "includes.h"
#include "Resource.h"
#include "Animation.h"

class Sprite;
typedef boost::shared_ptr<Sprite> spSprite;

class Sprite : public Resource
{
   public:
      Sprite(string id);
      ~Sprite();

      virtual void load(ticpp::Element* xml);

      void draw(int x, int y);

      void playAnimation(string id);

   private:
      map<string, spAnimation> _animations;
      spAnimation _current;
      string defaultAnimation;
      SDL_Surface* _image;
};

#endif
