#include "includes.h"
#include "PlayerJoinEvent.h"

BOOST_CLASS_EXPORT_GUID(xcore::PlayerJoinEvent, "PLAYER_JOIN_EVENT")

namespace xcore
{

PlayerJoinEvent::PlayerJoinEvent(string playerName, uint32 playerID) :
   Event(PLAYER_JOIN),
   _playerName(playerName),
   _playerID(playerID)
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

void PlayerJoinEvent::process(IEventListener* handler)
{
   handler->handleEvent(*this);
}

} // namespace
