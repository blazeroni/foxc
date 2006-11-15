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

spUnit EntityFactory::makeUnit(int playerID, spMapTile tile, int playerNum, string name) 
{
   spUnit u = Unit::makeUnit(++_lastEntityID, playerID, tile, playerNum, name);
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
spItem EntityFactory::makeRPG() { return makeItemHelper<RPG>(); }
spItem EntityFactory::makeRocket() { return makeItemHelper<Rocket>(); }
spItem EntityFactory::makeStimpack() { return makeItemHelper<Stimpack>(); }
spItem EntityFactory::makeRifleClip() { return makeItemHelper<RifleClip>(); }
spItem EntityFactory::makeMelee() { return makeItemHelper<Melee>(); }

spItem EntityFactory::makeItem( itemtype item )
{
   spItem i;
   // switches are so ugly when they're one line each
   // let it be noted i just lost 20 mins of work because i forgot to put break after medkit
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
      case MEDKIT:
         i = makeMedKit();
         break;
      case RPGL:
         i = makeRPG();
         break;
      case ROCKET:
         i = makeRocket();
         break;
      case STIM:
         i = makeStimpack();
         break;
      case RIFLE:
         i = makeRifle();
         break;
      case RIFLECLIP:
         i = makeRifleClip();
         break;
      case MELEE:
         i = makeMelee();
         break;
      case NONE:
      default:
         i = spItem();
         break;
   }
   return i;
}


} // namespace
