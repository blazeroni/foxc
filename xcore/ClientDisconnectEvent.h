#ifndef XCORE_CLIENTDISCONNECTEVENT_H
#define XCORE_CLIENTDISCONNECTEVENT_H

#include "includes.h"
#include "Event.h"
#include "IEventListener.h"

namespace xcore
{

class IEventListener;

class ClientDisconnectEvent : public Event
{
   friend class boost::serialization::access;
   public:
      ClientDisconnectEvent(string reason = "", uint32 reasonID = 0);
      virtual ~ClientDisconnectEvent();

      string getReason() const;

      virtual void process(IEventListener* handler);

      template<class T>
      void serialize(T & archive, const unsigned int version);

   private:
      string _reason;
      uint32 _reasonID; // currently unused
};

template<class T>
void ClientDisconnectEvent::serialize(T & archive, const unsigned int version)
{
   archive & boost::serialization::base_object<Event>(*this);
   archive & _reason;
   archive & _reasonID;
}

} // namespace

#endif
