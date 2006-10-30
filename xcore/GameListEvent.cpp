#include "includes.h"
#include "GameListEvent.h"

BOOST_CLASS_EXPORT_GUID(xcore::GameListEvent, "GAME_LIST_EVENT")

namespace xcore
{

GameListEvent::GameListEvent() :
   Event(GAME_LIST),
   _games()
{
}

GameListEvent::GameListEvent(list<string> games) :
   Event(GAME_LIST),
   _games(games)
{
}

GameListEvent::~GameListEvent()
{
}

const list<string>& GameListEvent::getGames()
{
   return _games;
}

void GameListEvent::process(IEventListener* handler)
{
   handler->handleEvent(*this);
}

} // namespace
