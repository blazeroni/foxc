#include "includes.h"
#include "Animation.h"
#include "Display.h"

Animation::Animation(string id) :
   _id(id)
{
}

Animation::~Animation()
{

}

string Animation::getID()
{
   return _id;
}

void Animation::load(ticpp::Element* element, int offsetX, int offsetY)
{
   element->GetAttribute("id", &_id);

   SDL_Rect clip;

   element->GetAttribute("width", &clip.w);
   element->GetAttribute("height", &clip.h);
   element->GetAttribute("startX", &clip.x);
   element->GetAttribute("startY", &clip.y);

   uint32 duration;
   uint16 count = 0;
   int adjustX;
   int adjustY;

   element->GetAttribute("count", &count);
   element->GetAttribute("duration", &duration);
   element->GetAttributeOrDefault("adjustX", &adjustX, 0);
   element->GetAttributeOrDefault("adjustY", &adjustY, 0);

   string loop = "";
   string type = "";

   element->GetAttributeOrDefault("loop", &loop, "false");

   _loop = (loop == "true") ? true : false;

   element->GetAttributeOrDefault("loopType", &type, "none");

   if (type == "reverse")
   {
      _loopType = LT_REVERSE;
   }
   else if (type == "circular")
   {
      _loopType = LT_CIRCULAR;
   }
   else
   {
      _loopType = LT_NONE;
   }

   uint16 maxID = count - 1;
   map<uint32, spAnimationFrame> tmpFrames;
   uint32 reserve = (_loopType == LT_REVERSE) ? (2*count) - 2 : count;
   for (uint16 i = 0; i < count; i++)
   {  
      SDL_Rect tmp = clip;
      tmp.x = tmp.x + (offsetX * (i));
      tmp.y = tmp.y + (offsetY * (i));

      tmpFrames[i] = spAnimationFrame(new AnimationFrame(i, tmp, duration, adjustX, adjustY));
      if (_loopType == LT_REVERSE && i != 0 && i != maxID)
      {
         uint32 id = maxID + (maxID - i);
         tmpFrames[id] = spAnimationFrame(new AnimationFrame(id, tmp, duration, adjustX, adjustY));
      }
   }

   for (uint16 i = 0; i < tmpFrames.size(); i++)
   {
      spAnimationFrame f = tmpFrames[i];
      if (i != tmpFrames.size() - 1)
      {
         f->setNext(wpAnimationFrame(tmpFrames[i+1]));
      }
      else if (_loop)
      {
         f->setNext(wpAnimationFrame(tmpFrames[0]));
      }
      _frames[i] = f;
   }

   _first = _frames[0];
}


void Animation::draw(SDL_Surface* image, int x, int y)
{
   _current->update(this);
   _current->draw(image, x, y);
}

void Animation::start()
{
   _current = _first;
   _current->_start = 0;
}

Animation::AnimationFrame::AnimationFrame(uint32 id, SDL_Rect clip, uint32 duration, int adjustX, int adjustY) :
   _id(id),
   _clip(clip),
   _start(0),
   _duration(duration),
   _adjustX(adjustX),
   _adjustY(adjustY)
{
}

void Animation::AnimationFrame::update(Animation* animation)
{
   uint32 ticks = SDL_GetTicks();
   
   if (_start == 0)
   {
      _start = ticks;
   }
   else if (ticks - _start > _duration)
   {
      spAnimationFrame nf = spAnimationFrame(_next);
      nf->_start = ticks;
      animation->_current = nf;
   }
}

void Animation::AnimationFrame::draw(SDL_Surface* image, int x, int y)
{
   Display::instance().draw(x + _adjustX - _clip.w/2, y + _adjustY - _clip.h/2, image, &_clip);
}

void Animation::AnimationFrame::setNext(wpAnimationFrame next)
{
   _next = next;
}