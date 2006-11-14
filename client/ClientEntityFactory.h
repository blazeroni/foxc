#ifndef XCLIENT_CLIENTENTITYFACTORY_H
#define XCLIENT_CLIENTENTITYFACTORY_H

#include "includes.h"
#include "xcore/EntityFactory.h"
#include "xcore/Singleton.h"
#include "ClientItem.h"

class ClientEntityFactory : public EntityFactory, public Singleton<ClientEntityFactory>
{
   friend class Singleton<ClientEntityFactory>;
   public:
      virtual ~ClientEntityFactory();

      virtual spUnit makeUnit(int playerID, spMapTile tile);
      virtual spWall makeWall(WALL_TYPE type);
      virtual spDoor makeDoor(WALL_TYPE type, bool open = false);
      virtual spItem makePistol();
      virtual spItem makeRifle();
      virtual spItem makeRPG();
      virtual spItem makeRocket();
      virtual spItem makeGrenade();
      virtual spItem makeMedKit();
      virtual spItem makePistolClip();

      //virtual spMapTile makeMapTile(TerrainType type, int x, int y);
      //virtual spWall makeWall(const Direction& dir);

   private:
      ClientEntityFactory();

/*      template <class T>
      spItem makeItemHelper();  */    
};

//template <class T, class U>
//spItem ClientEntityFactory::makeItemHelper()
//{
//   spItem item = spItem(new U(0));
//   spItem adapter = spItem(new T(++_lastEntityID, item));
//   add(adapter);
//   return adapter;
//}

#endif
