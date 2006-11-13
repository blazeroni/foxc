#include "includes.h"
#include "PlayerJoinEvent.h"

BOOST_CLASS_EXPORT_GUID(xcore::PlayerJoinEvent, "PLAYER_JOIN_EVENT")

namespace xcore
{

PlayerJoinEvent::PlayerJoinEvent(string playerName, uint32 playerID, uint16 num) :
   Event(PLAYER_JOIN),
   _playerName(playerName),
   _playerID(playerID),
   _playerNum(num)
{
}

PlayerJoinEvent::~PlayerJoinEvent()
{
}

string PlayerJoinEvent::getPlayerName() const
{
   return _playerName;
}

uint32 PlayerJoinEvent::getPlayerID() const
{
   return _playerID;
}

uint16 PlayerJoinEvent::getPlayerNumber() const
{
   return _playerNum;
}

void PlayerJoinEvent::process(IEventListener* handler)
{
   handler->handleEvent(*this);
}

} // namespace
