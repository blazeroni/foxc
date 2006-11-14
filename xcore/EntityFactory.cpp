#include "includes.h"
#include "EntityFactory.h"

namespace xcore
{

EntityFactory::EntityFactory(uint32 gameID) :
   _gameID(gameID),
   _lastEntityID(0)
{
}

EntityFactory::~EntityFactory()
{
}

void EntityFactory::resetEntities()
{
   _lastEntityID = 0;
}

spUnit EntityFactory::makeUnit(int playerID, spMapTile tile) 
{
   spUnit u = Unit::makeUnit(++_lastEntityID, playerID, tile);
   return u;
}

spWall EntityFactory::makeWall(WALL_TYPE type)
{
   spWall wall = spWall(new Wall(++_lastEntityID, type));
   return wall;
}
spDoor EntityFactory::makeDoor(WALL_TYPE type, bool open)
{
   spDoor door = spDoor(new Door(++_lastEntityID, type, open));
   return door;
}

spItem EntityFactory::makePistol()
{
   return makeItemHelper<Pistol>();
}

spItem EntityFactory::makeRifle()
{
   return makeItemHelper<Rifle>();
}
 
spItem EntityFactory::makeGrenade()
{
   return makeItemHelper<Grenade>();
}
      
spItem EntityFactory::makeMedKit()
{
   return makeItemHelper<MedKit>();
}

spItem EntityFactory::makePistolClip()
{
   return makeItemHelper<PistolClip>();
}

spItem EntityFactory::makeItem( itemtype item )
{
   spItem i;
   switch (item)
   {
      case PISTOL:
         i = makePistol();
         break;
      case PISTOLCLIP:
         i = makePistolClip();
         break;
      case GRENADE:
         i = makeGrenade();
         break;
      case NONE:
      default:
         i = spItem();
         break;
   }
   return i;
}


} // namespace
