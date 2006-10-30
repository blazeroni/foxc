#include "includes.h"
#include "ClientConnectEvent.h"

BOOST_CLASS_EXPORT_GUID(xcore::ClientConnectEvent, "CLIENT_CONNECT_EVENT")

namespace xcore
{

ClientConnectEvent::ClientConnectEvent(uint32 playerID) :
   Event(CLIENT_CONNECT),
   _playerID(playerID)
{
}

ClientConnectEvent::~ClientConnectEvent()
{
}

uint32 ClientConnectEvent::getPlayerID() const
{
   return _playerID;
}

void ClientConnectEvent::process(IEventListener* handler)
{
   handler->handleEvent(*this);
}

} // namespace
