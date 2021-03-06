#include "includes.h"
#include "Door.h"
#include "UnsupportedOperationException.h"

#define DOOR_HEALTH 100

namespace xcore
{

Door::Door(uint32 entityID, WALL_TYPE type, bool open) :
   MapObject(entityID, DOOR_HEALTH),
   _open(open),
   _type(type),
   _destroyed(false)
{
}

Door::~Door()
{
}

bool Door::canUse()
{
   return true;
}

void Door::use()
{
   _open = !_open;
}

string Door::getName()
{
   return "Door";
}

bool Door::isPassable() const
{
    return _open || _destroyed;
}

void Door::destroy()
{
   _destroyed = true;
}

bool Door::isDestroyed() const
{
   return _destroyed;
}

void Door::draw(Point position, Point dimensions) const
{
   throw UnsupportedOperationException();
}

} // namespace
