#ifndef XCLIENT_CLIENTWALL_H
#define XCLIENT_CLIENTWALL_H

#include "includes.h"
#include "xcore/Wall.h"

class ClientWall : public Wall
{
   public:
      ClientWall(uint32 entityID, WALL_TYPE type);
      ~ClientWall();

      void draw(Point position, Point dimensions) const;

   private:
      SDL_Surface* _image;

      static SDL_Surface* _none;
      static SDL_Surface* _ne;
      static SDL_Surface* _nw;
      static SDL_Surface* _se;
      static SDL_Surface* _sw;
      static SDL_Surface* _nesw;
      static SDL_Surface* _nwse;
      static SDL_Surface* _tse;
      static SDL_Surface* _tsw;
      static SDL_Surface* _tnw;
      static SDL_Surface* _tne;
      static SDL_Surface* _le;
      static SDL_Surface* _lw;
      static SDL_Surface* _ln;
      static SDL_Surface* _ls;
      static SDL_Surface* _all;

};

#endif
