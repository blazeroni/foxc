#include "includes.h"
#include "UnitFireEvent.h"

BOOST_CLASS_EXPORT_GUID(xcore::UnitFireEvent, "UNIT_FIRE_EVENT")

namespace xcore
{

UnitFireEvent::UnitFireEvent(uint32 unitID, uint32 x, uint32 y, uint32 hand) :
   Event(UNIT_FIRE),
   _unitID(unitID),
   _x(x),
   _y(y),
   _hand(hand)
{
}

UnitFireEvent::~UnitFireEvent()
{
}

uint32 UnitFireEvent::getUnitID() const
{
   return _unitID;
}

uint32 UnitFireEvent::getX() const
{
   return _x;
}

uint32 UnitFireEvent::getY() const
{
   return _y;
}

uint32 UnitFireEvent::getHand() const
{
   return _hand;
}

void UnitFireEvent::process(IEventListener* handler)
{
   handler->handleEvent(*this);
}

} // namespace
