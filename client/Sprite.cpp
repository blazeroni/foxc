#include "includes.h"
#include "Sprite.h"
#include "Display.h"

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

      if (id == defaultAnimation)
      {
         _current = anim;
         _current->start();
      }
   }
}

void Sprite::draw(int x, int y)
{
   _current->draw(_image, x, y);
}

void Sprite::playAnimation(string id)
{
   _animations[id]->start();
}