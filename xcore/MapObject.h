#ifndef XCORE_MAPOBJECT_H
#define XCORE_MAPOBJECT_H

#include "includes.h"

namespace xcore 
{
   class MapObject;
   typedef boost::shared_ptr<MapObject> spMapObject;
}

#include "GameEntity.h"
#include "Point.h"
#include "Direction.h"

namespace xcore 
{

enum dir { NE = 1, NW = 2, SE = 4, SW = 8 };

/// Represents objects that can be put onto MapTiles
class MapObject : public GameEntity
{
   public:
      MapObject(uint32 gameID);
      virtual ~MapObject();
      
      virtual bool isPassable() const = 0;
      virtual bool isPassable(const Direction& dir) const = 0;
      virtual void destroy() = 0;

      virtual void draw(Point position, Point dimensions) const = 0;
};

} // namespace

#endif
