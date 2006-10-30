#include "includes.h"
#include "GameOverEvent.h"

BOOST_CLASS_EXPORT_GUID(xcore::GameOverEvent, "GAME_OVER_EVENT")

namespace xcore
{

GameOverEvent::GameOverEvent(uint32 winner, uint32 gameID) :
   Event(GAME_OVER),
   _winner(winner),
   _gameID(gameID)
{
}

GameOverEvent::~GameOverEvent()
{
}

uint32 GameOverEvent::getWinnerID() const
{
   return _winner;
}

uint32 GameOverEvent::getGameID() const
{
   return _gameID;
}

void GameOverEvent::process(IEventListener* handler)
{
   handler->handleEvent(*this);
}

} // namespace
