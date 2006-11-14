#include "includes.h"
#include "GameHostEvent.h"

BOOST_CLASS_EXPORT_GUID(xcore::GameHostEvent, "GAME_HOST_EVENT")

namespace xcore
{

GameHostEvent::GameHostEvent(string gameName, string mapName) :
   Event(GAME_HOST),
   _gameName(gameName),
   _mapName(mapName)
{
}

GameHostEvent::~GameHostEvent()
{
}

string GameHostEvent::getGameName()
{
   return _gameName;
}

string GameHostEvent::getMapName()
{
   return _mapName;
}

void GameHostEvent::process(IEventListener* handler)
{
   handler->handleEvent(*this);
}

} // namespace
