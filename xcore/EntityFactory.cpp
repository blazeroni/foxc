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

spUnit EntityFactory::makeUnit(int playerID, spMapTile tile) 
{
   spUnit u = Unit::makeUnit(++_lastEntityID, playerID, tile);
   add(u);
   return u;
}

spWall EntityFactory::makeWall(WALL_TYPE type)
{
   spWall wall = spWall(new Wall(++_lastEntityID, type));
   //add(wall);
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

spGameEntity EntityFactory::get(uint32 entityID)
{
   /*if (_entities.find(entityID) != _entities.end())
   {
      return spGameEntity(_entities[entityID]);
   }*/
   return spGameEntity();
}

void EntityFactory::add(spGameEntity entity)
{
   wpGameEntity wp(entity);
   //_entities[_lastEntityID] = entity;
}


} // namespace
