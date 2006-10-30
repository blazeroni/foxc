#ifndef XCORE_WALL_H
#define XCORE_WALL_H

#include "MapObject.h"
#include "MapTile.h"
#include "Direction.h"
#include "Point.h"

namespace xcore 
{

class Wall;
typedef boost::shared_ptr<Wall> spWall;

class Wall : public MapObject
{
   public:
      Wall(uint32 entityID, const Direction& dir);
      ~Wall();

      bool isPassable() const;
      bool isPassable(const Direction& dir) const;
      void destroy();
      spMapTile tile1;
      spMapTile tile2;

      void draw(Point position, Point dimensions) const;

   private:
      bool _destroyed;
      Direction _dir;
/*      static SDL_Surface* _image;
      static SDL_Surface* _imageNE;
      static SDL_Surface* _imageNW;
      static SDL_Surface* _imageSE;
      static SDL_Surface* _imageSW;
*/
};

} // namespace

#endif
