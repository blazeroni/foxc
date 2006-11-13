#include "includes.h"
#include "GameListEvent.h"

BOOST_CLASS_EXPORT_GUID(xcore::GameListEvent, "GAME_LIST_EVENT")
BOOST_CLASS_EXPORT_GUID(xcore::GameListItem, "GAME_LIST_ITEM")

namespace xcore
{

GameListEvent::GameListEvent() :
   Event(GAME_LIST),
   _games()
{
}

GameListEvent::GameListEvent(vector<GameListItem> games) :
   Event(GAME_LIST),
   _games(games)
{
}

GameListEvent::~GameListEvent()
{
}

const vector<GameListItem>& GameListEvent::getGames()
{
   return _games;
}

void GameListEvent::process(IEventListener* handler)
{
   handler->handleEvent(*this);
}

} // namespace
