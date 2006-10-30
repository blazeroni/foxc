#ifndef XCORE_UNITINVSWAPEVENT_H
#define XCORE_UNITINVSWAPEVENT_H

#include "includes.h"
#include "IEventListener.h"
#include "Event.h"

namespace xcore
{

class IEventListener;

class UnitInvSwapEvent : public Event
{
   friend class boost::serialization::access;
   public:
      UnitInvSwapEvent(uint32 unitID = 0, int slot1 = 0, int slot2 = 0);
      virtual ~UnitInvSwapEvent();

      uint32 getUnitID() const;
      int getSlot1() const;
      int getSlot2() const;

      virtual void process(IEventListener* handler);

      template<class T>
      void serialize(T & archive, const unsigned int version);

   private:
      uint32 _unitID;
      int _slot1;
      int _slot2;
};

template<class T>
void UnitInvSwapEvent::serialize(T & archive, const unsigned int version) {
   archive & boost::serialization::base_object<Event>(*this);
   archive & _unitID;
   archive & _slot1;
   archive & _slot2;
}

} // namespace

#endif
