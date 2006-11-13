#ifndef XCORE_UNITFIREEVENT_H
#define XCORE_UNITFIREEVENT_H

#include "includes.h"
#include "IEventListener.h"
#include "Event.h"

namespace xcore
{

class IEventListener;

class UnitFireEvent : public Event
{
   friend class boost::serialization::access;
   public:
      UnitFireEvent(uint32 unitID = 0, uint32 x = 0, uint32 y = 0, uint32 hand = 0);
      virtual ~UnitFireEvent();

      uint32 getUnitID() const;
      uint32 getX() const;
      uint32 getY() const;
      uint32 getHand() const;

      virtual void process(IEventListener* handler);

      template<class T>
      void serialize(T & archive, const unsigned int version);

   private:
      uint32 _unitID;
      uint32 _x;
      uint32 _y;
      uint32 _hand;
};

template<class T>
void UnitFireEvent::serialize(T & archive, const unsigned int version) {
   archive & boost::serialization::base_object<Event>(*this);
   archive & _unitID;
   archive & _x;
   archive & _y;
   archive & _hand;
}

} // namespace

#endif
