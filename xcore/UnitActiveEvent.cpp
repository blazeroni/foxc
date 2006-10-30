#include "includes.h"
#include "UnitActiveEvent.h"

BOOST_CLASS_EXPORT_GUID(xcore::UnitActiveEvent, "UNIT_ACTIVE_EVENT")

namespace xcore
{

UnitActiveEvent::UnitActiveEvent(uint32 unitID, uint32 regenSteps) :
   Event(UNIT_ACTIVE),
   _unitID(unitID),
   _regenSteps(regenSteps)
{
}

UnitActiveEvent::~UnitActiveEvent()
{
}

uint32 UnitActiveEvent::getUnitID() const
{
   return _unitID;
}

uint32 UnitActiveEvent::getNumberOfRegens() const
{
   return _regenSteps;
}

void UnitActiveEvent::process(IEventListener* handler)
{
   handler->handleEvent(*this);
}

} // namespace
