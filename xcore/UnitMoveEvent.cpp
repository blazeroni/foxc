#include "includes.h"
#include "UnitMoveEvent.h"

BOOST_CLASS_EXPORT_GUID(xcore::UnitMoveEvent, "UNIT_MOVE_EVENT")

namespace xcore
{

UnitMoveEvent::UnitMoveEvent(uint32 unitID, uint32 x, uint32 y) :
   Event(UNIT_MOVE),
   _unitID(unitID),
   _x(x),
   _y(y)
{
}

UnitMoveEvent::~UnitMoveEvent()
{
}

uint32 UnitMoveEvent::getUnitID() const
{
   return _unitID;
}

uint32 UnitMoveEvent::getX() const
{
   return _x;
}

uint32 UnitMoveEvent::getY() const
{
   return _y;
}

void UnitMoveEvent::process(IEventListener* handler)
{
   handler->handleEvent(*this);
}

} // namespace
