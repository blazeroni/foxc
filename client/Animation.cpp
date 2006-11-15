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


void Animation::draw(SDL_Surface* image, spAnimationFrame frame, int x, int y)
{
   frame->draw(image, x, y);
}

Animation::AnimationFrame::AnimationFrame(uint32 id, SDL_Rect clip, uint32 duration, int adjustX, int adjustY) :
   _id(id),
   _clip(clip),
   _duration(duration),
   _adjustX(adjustX),
   _adjustY(adjustY)
{
}

void Animation::AnimationFrame::getCurrentFrame(spAnimationFrame& current, uint32& start)
{
   uint32 ticks = SDL_GetTicks();
   
   if (start == 0)
   {
      start = ticks;
   }
   else if (ticks - start > _duration)
   {
      if (!_next.expired())
      {
         spAnimationFrame nf = spAnimationFrame(_next);
         start = ticks;
         //return nf;
         current = nf;
      } 
      else 
      {
         current = spAnimationFrame();
         //animation->_done = true;
      }
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


LightweightAnimation::LightweightAnimation(spAnimation animation) :
   _heavy(animation),
   _start(0),
   _done(false)
{
   _current = _heavy->_first;
}

void LightweightAnimation::draw(SDL_Surface* image, int x, int y)
{
   _current->getCurrentFrame(_current, _start);
   if (_current.get())
   {
      _heavy->draw(image, _current, x, y);
   }
   else
   {
      _done = true;
   }
}

void LightweightAnimation::start()
{
   _current = _heavy->_first;
   _current->_start = 0;
}


bool LightweightAnimation::isDone()
{
   return _done;
}
