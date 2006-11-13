#include "includes.h"
#include "EventManager.h"

namespace xcore
{

EventManager::EventManager()
{
}

EventManager::~EventManager()
{
}

void EventManager::trigger(Event* e)
{
   multimap<EVENT_TYPE, IEventListener*>::iterator iter = eventListenerMap.find(e->getType());
   if (iter != eventListenerMap.end())
   {
      e->process(iter->second);
   }
}

void EventManager::queue(Event* e)
{

}

void EventManager::removeListener(IEventListener* handler)
{
   multimap<EVENT_TYPE, IEventListener*>::iterator iter;
   for(iter = eventListenerMap.begin(); iter != eventListenerMap.end(); )
   {
      if (iter->second == handler) 
      {
         eventListenerMap.erase(iter++);
      } 
      else 
      {
         ++iter;
      }
   }
}

} // namespace
