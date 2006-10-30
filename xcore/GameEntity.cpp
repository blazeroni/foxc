#include "includes.h"
#include "GameEntity.h"

namespace xcore
{

GameEntity::GameEntity(uint32 entityID) :
   _id(entityID)
{
}

GameEntity::~GameEntity()
{
}

uint32 GameEntity::getID() const
{
   return _id;
}

} // namespace
