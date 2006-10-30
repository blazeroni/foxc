#include "includes.h"
#include "StartGameEvent.h"

BOOST_CLASS_EXPORT_GUID(xcore::StartGameEvent, "START_GAME_EVENT")

namespace xcore
{

StartGameEvent::StartGameEvent() :
   Event(START_GAME)
{
}

StartGameEvent::~StartGameEvent()
{
}

void StartGameEvent::process(IEventListener* handler)
{
   handler->handleEvent(*this);
}

} // namespace
