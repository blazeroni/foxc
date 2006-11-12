#include "includes.h"
#include "MapObject.h"

namespace xcore
{

MapObject::MapObject(uint32 gameID) :
   GameEntity(gameID)
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

} // namespace
