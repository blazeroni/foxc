#ifndef XCORE_EVENTMANAGER_H
#define XCORE_EVENTMANAGER_H

#include "includes.h"
#include "Singleton.h"
#include "IEventListener.h"
#include "Event.h"

namespace xcore
{

class EventManager : public Singleton<EventManager>
{
   friend class Singleton<EventManager>;

   public:
      ~EventManager();

      template <class T>
      void addListener(IEventListener* handler);//, const EventType& type);
      void removeListener(IEventListener* handler);

      void trigger(Event* e);
      void queue(Event* e);

   private:
      EventManager();

      multimap<EVENT_TYPE, IEventListener*> eventListenerMap;

};

template <class T>
void EventManager::addListener(IEventListener* handler)
{
   // this is kind of a strange use of templates... because all the events have a default constructor for 
   //    serialization purposes, this takes advantage of that to create an instance and get the event type
   eventListenerMap.insert(pair<EVENT_TYPE, IEventListener*>(T().getType(), handler));
}

} // namespace

#endif
