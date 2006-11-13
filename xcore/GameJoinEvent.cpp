#include "includes.h"
#include "GameJoinEvent.h"

BOOST_CLASS_EXPORT_GUID(xcore::GameJoinEvent, "GAME_JOIN_EVENT")

namespace xcore
{

GameJoinEvent::GameJoinEvent(string gameName, uint32 points, uint16 playerNum) :
   Event(GAME_JOIN),
   _gameName(gameName),
   _points(points),
   _joined(false),
   _playerNum(playerNum)
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

uint16 GameJoinEvent::getPlayerNumber() const
{
   return _playerNum;
}

void GameJoinEvent::setPlayerNumber(uint16 num) 
{
   _playerNum = num;
}

void GameJoinEvent::setMaxPoints(uint32 points)
{
   _points = points;
}

uint32 GameJoinEvent::getMaxPoints() const
{
   return _points;
}

void GameJoinEvent::process(IEventListener* handler)
{
   handler->handleEvent(*this);
}

} // namespace
