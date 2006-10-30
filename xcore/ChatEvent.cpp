#include "includes.h"
#include "ChatEvent.h"

BOOST_CLASS_EXPORT_GUID(xcore::ChatEvent, "CHAT_EVENT")

namespace xcore
{

ChatEvent::ChatEvent(string message, string playerName) :
   Event(CHAT),
   _message(message),
   _playerName(playerName)
{
}

ChatEvent::~ChatEvent()
{
}

void ChatEvent::process(IEventListener* handler)
{
   handler->handleEvent(*this);
}

string ChatEvent::getMessage()
{
   return _message;
}

string ChatEvent::getPlayerName()
{
   return _playerName;
}

} // namespace
