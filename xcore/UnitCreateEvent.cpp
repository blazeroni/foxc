#include "includes.h"
#include "UnitCreateEvent.h"

BOOST_CLASS_EXPORT_GUID(xcore::UnitCreateEvent, "UNIT_CREATE_EVENT")

namespace xcore
{

UnitCreateEvent::UnitCreateEvent(uint32 playerID, uint32 x, uint32 y) :
   Event(UNIT_CREATE),
   _playerID(playerID),
   _x(x),
   _y(y)
{
}

UnitCreateEvent::~UnitCreateEvent()
{
}

uint32 UnitCreateEvent::getPlayerID() const
{
   return _playerID;
}

uint32 UnitCreateEvent::getX() const
{
   return _x;
}

uint32 UnitCreateEvent::getY() const
{
   return _y;
}

void UnitCreateEvent::process(IEventListener* handler)
{
   handler->handleEvent(*this);
}

} // namespace
