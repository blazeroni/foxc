#include "includes.h"
#include "Wall.h"
#include "UnsupportedOperationException.h"

#define WALL_HEALTH 100


namespace xcore
{

Wall::Wall(uint32 entityID, WALL_TYPE type) :
   MapObject(entityID, WALL_HEALTH),
   _type(type),
   _destroyed(false)
{

}

Wall::~Wall()
{
}

bool Wall::isPassable() const
{
    return _destroyed;
}

void Wall::destroy()
{
   _destroyed = true;
   // @ todo fill in later
}

bool Wall::isDestroyed() const
{
   return _destroyed;
}

void Wall::draw(Point position, Point dimensions) const
{
   throw UnsupportedOperationException();
}

void Wall::removeType(WALL_DIRECTION dir)
{
   if (!_destroyed)
   {
      _type = WALL_TYPE(_type - dir);
   }
}


} // namespace
