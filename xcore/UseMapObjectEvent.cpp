#include "includes.h"
#include "UseMapObjectEvent.h"

BOOST_CLASS_EXPORT_GUID(xcore::UseMapObjectEvent, "USE_MAP_OBJECT_EVENT")

namespace xcore
{

UseMapObjectEvent::UseMapObjectEvent(uint32 unitID) :
   Event(USE_MAP_OBJECT),
   _unitID(unitID)
{
}

UseMapObjectEvent::~UseMapObjectEvent()
{
}

uint32 UseMapObjectEvent::getUnitID() const
{
   return _unitID;
}

void UseMapObjectEvent::process(IEventListener* handler)
{
   handler->handleEvent(*this);
}

} // namespace
