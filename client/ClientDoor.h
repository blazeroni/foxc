#ifndef XCLIENT_CLIENTDOOR_H
#define XCLIENT_CLIENTDOOR_H

#include "includes.h"
#include "xcore/Door.h"

class ClientDoor : public Door
{
   public:
      ClientDoor(uint32 entityID, WALL_TYPE type, bool open = false);
      ~ClientDoor();

      void destroy();

      virtual void use();

      void draw(Point position, Point dimensions) const;

   private:
      void updateImage();

      SDL_Surface* _image;

      static SDL_Surface* _nwse_closed;
      static SDL_Surface* _nwse_open;
      static SDL_Surface* _nesw_closed;
      static SDL_Surface* _nesw_open;
      static SDL_Surface* _destroyed;
};

#endif
