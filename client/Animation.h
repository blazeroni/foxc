#ifndef XCLIENT_ANIMATION_H
#define XCLIENT_ANIMATION_H

#include "includes.h"
#include "Resource.h"

class Animation;
typedef boost::shared_ptr<Animation> spAnimation;

class LightweightAnimation;
typedef boost::shared_ptr<LightweightAnimation> spLightweightAnimation;

// circular: 1, 2, 3, 1, 2, 3,...  
// reverse: 1, 2, 3, 2, 1, 2, 3,... 
enum LOOP_TYPE { LT_CIRCULAR, LT_REVERSE, LT_NONE };

class Animation
{
   friend class LightweightAnimation;
   friend struct AnimationFrame;

   struct AnimationFrame;
   typedef boost::shared_ptr<AnimationFrame> spAnimationFrame;
   typedef boost::weak_ptr<AnimationFrame> wpAnimationFrame;

   public:
      Animation(string id);
      virtual ~Animation();

      string getID();

      virtual void load(ticpp::Element* element, int offsetX, int offsetY);

      void draw(SDL_Surface* image, spAnimationFrame frame, int x, int y);

      //void start();
      //void update(uint32);
      //void stop();

   private:
      map<uint32, spAnimationFrame> _frames;
      spAnimationFrame _first;
      //spAnimationFrame _current;
      string _id;
      bool _loop;
      LOOP_TYPE _loopType;

      struct AnimationFrame
      {
         AnimationFrame(uint32 id, SDL_Rect clip, uint32 duration, int adjustX, int adjustY);
         
         void draw(SDL_Surface* image, int x, int y);

         void setNext(wpAnimationFrame next);
         void getCurrentFrame(spAnimationFrame& current, uint32& start);

         int _adjustX;
         int _adjustY;

         uint32 _id;
         SDL_Rect _clip;
         uint32 _duration;
         uint32 _start;
         wpAnimationFrame _next;
         wpAnimationFrame _this;
      };
};

class LightweightAnimation
{
   public:
      explicit LightweightAnimation(spAnimation animation);

      void draw (SDL_Surface* image, int x, int y);

      void start();

      bool isDone();

   private:
      spAnimation _heavy;
      Animation::spAnimationFrame _current;
      uint32 _start;
      bool _done;
};

#endif