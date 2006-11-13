#include "includes.h"
#include "PlayerLeaveEvent.h"

BOOST_CLASS_EXPORT_GUID(xcore::PlayerLeaveEvent, "PLAYER_LEAVE_EVENT")

namespace xcore
{

PlayerLeaveEvent::PlayerLeaveEvent(string playerName, uint32 playerID) :
   Event(PLAYER_LEAVE),
   _playerName(playerName),
   _playerID(playerID)
{
}

PlayerLeaveEvent::~PlayerLeaveEvent()
{
}

string PlayerLeaveEvent::getPlayerName() const
{
   return _playerName;
}

uint32 PlayerLeaveEvent::getPlayerID() const
{
   return _playerID;
}

void PlayerLeaveEvent::process(IEventListener* handler)
{
   handler->handleEvent(*this);
}

} // namespace
