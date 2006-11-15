#ifndef XCLIENT_SPRITE_H
#define XCLIENT_SPRITE_H

#include "includes.h"
#include "Resource.h"
#include "Animation.h"
#include "xcore/Point.h"

class Sprite;
typedef boost::shared_ptr<Sprite> spSprite;

class LightweightSprite;
typedef boost::shared_ptr<LightweightSprite> spLightweightSprite;

class Sprite : public Resource
{
   friend class LightweightSprite;
   public:
      Sprite(string id);
      ~Sprite();

      virtual void load(ticpp::Element* xml);

      void draw(spLightweightAnimation anim, int x, int y);

      // hackish, oh well =/
      static void abandon(spLightweightSprite lightSprite, int x, int y);
      static void drawOrphans(Point offset);

      //void playAnimation(string id);

   private:
      map<string, spAnimation> _animations;
      //spAnimation _current;
      string defaultAnimation;
      SDL_Surface* _image;

      static list<spLightweightSprite> _orphans;
      static map<spLightweightSprite, Point> _orphanMap;
};

class LightweightSprite
{
   public:
      explicit LightweightSprite(spSprite sprite);

      void draw(int x, int y);

      void playAnimation(string id);

      bool isDone();

   private:
      spSprite _heavy;
      spLightweightAnimation _current;
};

#endif
