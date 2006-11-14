#include "includes.h"
#include "MapObject.h"

namespace xcore
{

MapObject::MapObject(uint32 gameID, int health) :
   GameEntity(gameID),
   _health(health)
{
}

MapObject::~MapObject()
{
}

bool MapObject::canUse()
{
   return false;
}

void MapObject::use()
{
   // do nothing
}

string MapObject::getName()
{
   return "MapObject";
}

void MapObject::takeDamage( int damage )
{
   _health -= damage;
   if (_health <= 0)
   {
      destroy();
   }
}

bool MapObject::isDestroyed() const
{
   return false;
}

} // namespace
