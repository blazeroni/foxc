#ifndef XCORE_CLIENTCONNECTEVENT_H
#define XCORE_CLIENTCONNECTEVENT_H

#include "includes.h"
#include "Event.h"
#include "IEventListener.h"

namespace xcore
{

class IEventListener;

class ClientConnectEvent : public Event
{
   friend class boost::serialization::access;
   public:
      ClientConnectEvent(uint32 playerID = 0);
      virtual ~ClientConnectEvent();

      uint32 getPlayerID() const;

      virtual void process(IEventListener* handler);

      template<class T>
      void serialize(T & archive, const unsigned int version);

   private:
      uint32 _playerID;
};

template<class T>
void ClientConnectEvent::serialize(T & archive, const unsigned int version)
{
   archive & boost::serialization::base_object<Event>(*this);
   archive & _playerID;
}

} // namespace

#endif
