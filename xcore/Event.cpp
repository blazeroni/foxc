#include "includes.h"
#include "Event.h"
#include "EventManager.h"

namespace xcore
{

//EventType::EventType(string id) :
//   _stringID(id),
//   _hashID(hash(id))
//{
//
//}
//
//EventType::~EventType()
//{
//
//}
//
//uint32 EventType::getID() const
//{
//   return _hashID;
//}
//
//string EventType::getStringID() const
//{
//   return _stringID;
//}
//
//bool EventType::operator==(EventType& et) const
//{
//   return (et._hashID == this->_hashID);
//}
//
//
//uint32 EventType::hash(string id)
//{
//   static boost::hash<const char*> h;
//   return h(id.c_str());
//}

Event::Event(EVENT_TYPE type) :
   _eventType(type),
   _socket((uint32)-1)
{
   _time = SDL_GetTicks();
}

Event::~Event()
{
}

EVENT_TYPE Event::getType() const
{
   return _eventType;
}

void Event::fire()
{
   EventManager::instance().trigger(this);
}

void Event::queue()
{
   EventManager::instance().queue(this);
}

bool Event::isLocal() const
{
   return _socket < 0;
}
      
uint32 Event::getSource() const
{
   return _socket;
}

void Event::setSource(int32 socket)
{
   _socket = socket;
}

} // namespace
