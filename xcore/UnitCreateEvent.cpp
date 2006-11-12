#include "includes.h"
#include "UnitCreateEvent.h"

BOOST_CLASS_EXPORT_GUID(xcore::UnitCreateEvent, "UNIT_CREATE_EVENT")

namespace xcore
{

UnitCreateEvent::UnitCreateEvent(uint32 playerID, uint32 x, uint32 y, 
	itemtype s0, itemtype s1, itemtype s2, itemtype s3, itemtype s4,
	itemtype s5, itemtype s6) :
   Event(UNIT_CREATE),
   _playerID(playerID),
   _x(x),
   _y(y),
   _s0(s0),
   _s1(s1),
   _s2(s2),
   _s3(s3),
   _s4(s4),
   _s5(s5),
   _s6(s6)
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
itemtype UnitCreateEvent::getS0() const { return _s0; }
itemtype UnitCreateEvent::getS1() const { return _s1; }
itemtype UnitCreateEvent::getS2() const { return _s2; }
itemtype UnitCreateEvent::getS3() const { return _s3; }
itemtype UnitCreateEvent::getS4() const { return _s4; }
itemtype UnitCreateEvent::getS5() const { return _s5; }
itemtype UnitCreateEvent::getS6() const { return _s6; }

void UnitCreateEvent::process(IEventListener* handler)
{
   handler->handleEvent(*this);
}

} // namespace
