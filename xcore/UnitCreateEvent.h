#ifndef XCORE_UNITCREATEEVENT_H
#define XCORE_UNITCREATEEVENT_H

#include "includes.h"
#include "Event.h"
#include "IEventListener.h"

namespace xcore
{

class IEventListener;

class UnitCreateEvent : public Event
{
   friend class boost::serialization::access;
   public:
      UnitCreateEvent(uint32 playerID=-1, uint32 x=0, uint32 y=0);
      virtual ~UnitCreateEvent();

      uint32 getPlayerID() const;
      uint32 getX() const;
      uint32 getY() const;

      virtual void process(IEventListener* handler);

      template<class T>
      void serialize(T & archive, const unsigned int version);

   private:
      uint32 _x;
      uint32 _y;
      uint32 _playerID;
      uint32 _temp;
};

template<class T>
void UnitCreateEvent::serialize(T & archive, const unsigned int version)
{
   archive & boost::serialization::base_object<Event>(*this);
   archive & _playerID;
   archive & _x;
   archive & _y;
   archive & _temp;
}

} // namespace

#endif
