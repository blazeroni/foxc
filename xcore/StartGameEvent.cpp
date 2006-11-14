#include "includes.h"
#include "StartGameEvent.h"

BOOST_CLASS_EXPORT_GUID(xcore::StartGameEvent, "START_GAME_EVENT")

namespace xcore
{

StartGameEvent::StartGameEvent(uint32 seed) :
   Event(START_GAME),
   _seed(seed)
{
}

StartGameEvent::~StartGameEvent()
{
}

void StartGameEvent::process(IEventListener* handler)
{
   handler->handleEvent(*this);
}

uint32 StartGameEvent::getSeed() const { return _seed; }

} // namespace
