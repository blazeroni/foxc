#include "includes.h"
#include "Item.h"
#include "UnsupportedOperationException.h"

namespace xcore
{

Item::Item(uint32 entityID) :
   GameEntity(entityID)
{
    _name = "Item";
    _effectiveRange = 0;
    _turnCost = 0;
//    _image = NULL;
}

string Item::getName() const
{
    return _name;
}

int Item::getTurnCost() const
{
    return _turnCost;
}

int Item::getRange() const
{
    return _effectiveRange;
}

int Item::use( spMapTile tile )
{
    return 0;
}

SDL_Surface* Item::getImage()
{
   throw UnsupportedOperationException();
}
SDL_Surface* Item::getInvImage()
{
   throw UnsupportedOperationException();
}

int Item::hasAmmo() const
{
    return -1;
}
string Item::getType() const
{
    return _type;
}
void Item::reload() {}

} // namespace
