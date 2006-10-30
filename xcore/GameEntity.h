#ifndef XCORE_GAMEENTITY_H
#define XCORE_GAMEENTITY_H

#include "includes.h"
#include "Point.h"

namespace xcore
{

class GameEntity;
typedef boost::shared_ptr<GameEntity> spGameEntity;
typedef boost::weak_ptr<GameEntity> wpGameEntity;

class GameEntity
{
   public:
      GameEntity(uint32 entityID);
      virtual ~GameEntity();

      uint32 getID() const;

      //virtual vector<string> getResourceIDs() = 0;

      //virtual void draw(Point position, Point dimensions) const = 0;

   private:
      uint32 _id;
};

}

#endif
