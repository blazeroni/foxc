#include "includes.h"
#include "ClientEntityFactory.h"
#include "ClientUnit.h"
#include "ClientMapTile.h"
#include "ClientWeapon.h"
#include "ClientWall.h"
#include "ClientDoor.h"

ClientEntityFactory::ClientEntityFactory() :
   EntityFactory(GAME_ID)
{
}

ClientEntityFactory::~ClientEntityFactory()
{
}

spUnit ClientEntityFactory::makeUnit(int playerID, spMapTile tile) 
{
   spUnit u = ClientUnit::makeUnit(++_lastEntityID, playerID, tile);
   return u;
}

spWall ClientEntityFactory::makeWall(WALL_TYPE type)
{
   spWall wall = spWall(new ClientWall(++_lastEntityID, type));
   return wall;
}

spDoor ClientEntityFactory::makeDoor(WALL_TYPE type, bool open)
{
   spDoor door = spDoor(new ClientDoor(++_lastEntityID, type, open));
   return door;
}

spItem ClientEntityFactory::makePistol()
{
   return makeItemHelper<ClientPistol>();
}

spItem ClientEntityFactory::makeRifle()
{
   return makeItemHelper<ClientRifle>();
}
spItem ClientEntityFactory::makeRPG() { return makeItemHelper<ClientRPG>(); }
spItem ClientEntityFactory::makeRocket() { return makeItemHelper<ClientRocket>(); }
 
spItem ClientEntityFactory::makeGrenade()
{
   return makeItemHelper<ClientGrenade>();
}
      
spItem ClientEntityFactory::makeMedKit()
{
   return makeItemHelper<ClientMedKit>();
}

spItem ClientEntityFactory::makePistolClip()
{
   return makeItemHelper<ClientPistolClip>();
}
