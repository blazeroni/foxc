#include "includes.h"
#include "GameJoinEvent.h"

BOOST_CLASS_EXPORT_GUID(xcore::GameJoinEvent, "GAME_JOIN_EVENT")

namespace xcore
{

GameJoinEvent::GameJoinEvent(string gameName, bool host) :
   Event(GAME_JOIN),
   _gameName(gameName),
   _host(host),
   _joined(false)
{
}

GameJoinEvent::~GameJoinEvent()
{
}

void GameJoinEvent::setJoined(bool ok)
{
   _joined = ok;
}

bool GameJoinEvent::hasJoined() const
{
   return _joined;
}

string GameJoinEvent::getGameName() const
{
   return _gameName;
}

bool GameJoinEvent::isHost() const
{
   return _host;
}

void GameJoinEvent::process(IEventListener* handler)
{
   handler->handleEvent(*this);
}

} // namespace
