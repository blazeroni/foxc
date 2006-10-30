#include "includes.h"
#include "UnitInvSwapEvent.h"

BOOST_CLASS_EXPORT_GUID(xcore::UnitInvSwapEvent, "UNIT_INVSWAP_EVENT")

namespace xcore
{

UnitInvSwapEvent::UnitInvSwapEvent(uint32 unitID, int slot1, int slot2) :
   Event(UNIT_INVSWAP),
   _unitID(unitID),
   _slot1(slot1),
   _slot2(slot2)
{
}

UnitInvSwapEvent::~UnitInvSwapEvent()
{
}

uint32 UnitInvSwapEvent::getUnitID() const
{
   return _unitID;
}

int UnitInvSwapEvent::getSlot1() const
{
   return _slot1;
}

int UnitInvSwapEvent::getSlot2() const
{
   return _slot2;
}

void UnitInvSwapEvent::process(IEventListener* handler)
{
   handler->handleEvent(*this);
}

} // namespace
