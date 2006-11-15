#include "includes.h"
#include "Sprite.h"
#include "Display.h"

list<spLightweightSprite> Sprite::_orphans;
map<spLightweightSprite, Point> Sprite::_orphanMap;

Sprite::Sprite(string id) :
   Resource(id),
   _image(NULL)
{
}

Sprite::~Sprite()
{
}

void Sprite::load(ticpp::Element* xml)
{
   string filename;
   bool useAlpha;
   int frameOffsetX = 0;
   int frameOffsetY = 0;
   
   xml->GetAttribute("filename", &filename);
   xml->GetAttributeOrDefault("useAlpha", &useAlpha, true);
   xml->GetAttribute("default", &defaultAnimation);
   xml->GetAttributeOrDefault("frameOffsetX", &frameOffsetX, 0);
   xml->GetAttributeOrDefault("frameOffsetY", &frameOffsetY, 0);

   _image = Display::instance().loadImage("resources/images/" + filename, useAlpha);

   ticpp::Iterator< ticpp::Element > child;
   for ( child = xml->FirstChildElement(); child != child.end(); child++ )
   {
      string id;
      child->GetAttribute("id", &id);

      spAnimation anim = spAnimation(new Animation(id));
      anim->load(&(*child), frameOffsetX, frameOffsetY);

      _animations[id] = anim;

      //if (id == defaultAnimation)
      //{
      //   _current = anim;
      //   _current->start();
      //}
   }
}

void Sprite::draw(spLightweightAnimation anim, int x, int y)
{
   anim->draw(_image, x, y);
}

//void Sprite::playAnimation(string id)
//{
//   _animations[id]->start();
//}

void Sprite::drawOrphans(Point offset)
{
   list<spLightweightSprite>::iterator iter;
   list<spLightweightSprite> remove;
   for (iter = _orphans.begin(); iter != _orphans.end(); ++iter)
   {
      Point loc = _orphanMap[(*iter)];
      (*iter)->draw(loc.x - offset.x, loc.y - offset.y);
      if ((*iter)->isDone())
      {
         remove.push_back(*iter);
      }
   }

   for (iter = remove.begin(); iter != remove.end(); ++iter)
   {
      _orphans.remove(*iter);
      _orphanMap.erase(*iter);
   }
}

void Sprite::abandon(spLightweightSprite sprite, int x, int y)
{
   _orphans.push_back(sprite);
   _orphanMap[sprite] = Point(x,y);
}

LightweightSprite::LightweightSprite(spSprite sprite) :
   _heavy(sprite)
{
   _current = spLightweightAnimation(new LightweightAnimation(_heavy->_animations[sprite->defaultAnimation]));
   _current->start();
}

LightweightSprite::LightweightSprite(spLightweightSprite ls) :
   _heavy(ls->_heavy)
{
   _current = spLightweightAnimation(new LightweightAnimation(ls->_current));
}

void LightweightSprite::draw(int x, int y)
{
   _heavy->draw(_current, x, y);
}

void LightweightSprite::playAnimation(string id)
{
   _current = spLightweightAnimation(new LightweightAnimation(_heavy->_animations[id]));
   _current->start();
}

bool LightweightSprite::isDone()
{
   return _current->isDone();
}

