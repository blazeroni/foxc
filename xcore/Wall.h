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

enum WALL_DIRECTION { WD_NW = 1, WD_NE = 2, WD_SE = 4, WD_SW = 8 };

enum WALL_TYPE { WT_NONE = 0,
                  WT_NW = 1,
                  WT_NE = 2,
                  WT_SE = 4,
                  WT_SW = 8,
                  WT_NE_SW = 10, 
                  WT_NW_SE = 5, 
                  WT_T_NE = 7, 
                  WT_T_NW = 11, 
                  WT_T_SE = 14, 
                  WT_T_SW = 13, 
                  WT_L_E = 6,
                  WT_L_S = 12,
                  WT_L_W = 9,
                  WT_L_N = 3,
                  WT_ALL = 15,
                  WT_DOOR = 16 };

class Wall : public MapObject
{
   public:
      Wall(uint32 entityID, WALL_TYPE type);
      ~Wall();

      bool isPassable() const;

      void destroy();
      bool isDestroyed() const;

      void draw(Point position, Point dimensions) const;

      // allows for destruction of adjacent walls
      virtual void removeType(WALL_DIRECTION wd);

   protected:
      WALL_TYPE _type;

   private:
      bool _destroyed;
};

} // namespace

#endif
