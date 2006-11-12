#ifndef XCORE_ENTITYFACTORY_H
#define XCORE_ENTITYFACTORY_H

#include "includes.h"
#include "Unit.h"
#include "MapTile.h"
#include "Wall.h"
#include "Weapon.h"
#include "Item.h"
#include "Door.h"

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

      spGameEntity get(uint32 entityID);

   protected: 
      uint32 checkID(uint32 id);
      void add(spGameEntity entity);
      uint32 _gameID;
      uint32 _lastEntityID;

      template <class T>
      spItem makeItemHelper();

   private:
      //map<unsigned long, wpGameEntity> _entities;
};

template <class T>
spItem EntityFactory::makeItemHelper()
{
   spItem item = spItem(new T(++_lastEntityID));
   add(item);
   return item;
}

} // namespace

#endif
