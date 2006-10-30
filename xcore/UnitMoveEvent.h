#ifndef XCORE_UNITMOVEEVENT_H
#define XCORE_UNITMOVEEVENT_H

#include "includes.h"
#include "IEventListener.h"
#include "Event.h"

namespace xcore
{

class IEventListener;

class UnitMoveEvent : public Event
{
   friend class boost::serialization::access;
   public:
      UnitMoveEvent(uint32 unitID = 0, uint32 x = 0, uint32 y = 0);
      virtual ~UnitMoveEvent();

      uint32 getUnitID() const;
      uint32 getX() const;
      uint32 getY() const;

      virtual void process(IEventListener* handler);

      template<class T>
      void serialize(T & archive, const unsigned int version);

   private:
      uint32 _unitID;
      uint32 _x;
      uint32 _y;
};

template<class T>
void UnitMoveEvent::serialize(T & archive, const unsigned int version) {
   archive & boost::serialization::base_object<Event>(*this);
   archive & _unitID;
   archive & _x;
   archive & _y;
}

} // namespace

#endif
