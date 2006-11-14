#ifndef XCORE_ENTITYFACTORY_H
#define XCORE_ENTITYFACTORY_H

#include "includes.h"
#include "Unit.h"
#include "MapTile.h"
#include "Wall.h"
#include "Door.h"
#include "Weapon.h"
#include "Item.h"

namespace xcore
{

class EntityFactory;
typedef boost::shared_ptr<EntityFactory> spEntityFactory;

class EntityFactory
{
   public:
      EntityFactory(uint32 gameID);
      virtual ~EntityFactory();

      virtual spUnit makeUnit(int playerID, spMapTile tile);
      virtual spWall makeWall(WALL_TYPE type);
      virtual spDoor makeDoor(WALL_TYPE type, bool open = false);
      virtual spItem makePistol();
      virtual spItem makeRifle();
      virtual spItem makeGrenade();
      virtual spItem makeMedKit();
      virtual spItem makePistolClip();
      virtual spItem makeRPG();
      virtual spItem makeRocket();
      virtual spItem makeItem( itemtype item );

      void resetEntities();

   protected: 
      uint32 checkID(uint32 id);
      uint32 _gameID;
      uint32 _lastEntityID;

      template <class T>
      spItem makeItemHelper();
};

template <class T>
spItem EntityFactory::makeItemHelper()
{
   spItem item = spItem(new T(++_lastEntityID));
   return item;
}

} // namespace

#endif
