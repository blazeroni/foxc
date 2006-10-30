#include "includes.h"
#include "ClientDisconnectEvent.h"

BOOST_CLASS_EXPORT_GUID(xcore::ClientDisconnectEvent, "CLIENT_DISCONNECT_EVENT")

namespace xcore
{

ClientDisconnectEvent::ClientDisconnectEvent(string reason, uint32 reasonID) :
   Event(CLIENT_DISCONNECT),
   _reason(reason),
   _reasonID(reasonID)
{
}

ClientDisconnectEvent::~ClientDisconnectEvent()
{
}

string ClientDisconnectEvent::getReason() const
{
   return _reason;
}

void ClientDisconnectEvent::process(IEventListener* handler)
{
   handler->handleEvent(*this);
}

} // namespace
