#include "includes.h"
#include "UnitWaitEvent.h"

BOOST_CLASS_EXPORT_GUID(xcore::UnitWaitEvent, "UNIT_WAIT_EVENT")

namespace xcore
{

UnitWaitEvent::UnitWaitEvent(uint32 unitID) :
   Event(UNIT_WAIT),
   _unitID(unitID)
{
}

UnitWaitEvent::~UnitWaitEvent()
{
}

uint32 UnitWaitEvent::getUnitID() const
{
   return _unitID;
}

void UnitWaitEvent::process(IEventListener* handler)
{
   handler->handleEvent(*this);
}

} // namespace
