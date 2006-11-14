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

//enum dir { NE = 1, NW = 2, SE = 4, SW = 8 };

/// Represents objects that can be put onto MapTiles
class MapObject : public GameEntity
{
   public:
      MapObject(uint32 entityID, int health = 1000);
      virtual ~MapObject();

      virtual string getName();

      virtual bool canUse();
      virtual void use();
      virtual bool isPassable() const = 0;
      virtual void destroy() = 0;
      virtual bool isDestroyed() const;
      virtual void takeDamage(int damage);

      virtual void draw(Point position, Point dimensions) const = 0;

   private:
      int _health;
};

} // namespace

#endif
