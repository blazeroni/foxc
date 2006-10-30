#ifndef XCORE_UNITACTIVEEVENT_H
#define XCORE_UNITACTIVEEVENT_H

#include "includes.h"
#include "Event.h"
#include "IEventListener.h"

namespace xcore
{

class IEventListener;

class UnitActiveEvent : public Event
{
   friend class boost::serialization::access;
   public:
      UnitActiveEvent(uint32 unitID = 0, uint32 regenSteps = 0);
      virtual ~UnitActiveEvent();

      uint32 getUnitID() const;
      uint32 getNumberOfRegens() const;

      virtual void process(IEventListener* handler);

      template<class T>
      void serialize(T & archive, const unsigned int version);

   private:
      uint32 _unitID;
      // number of times to regen units to get the active unit to have max action points
      uint32 _regenSteps;
};

template<class T>
void UnitActiveEvent::serialize(T & archive, const unsigned int version)
{
   archive & boost::serialization::base_object<Event>(*this);
   archive & _unitID;
   archive & _regenSteps;
}

} // namespace

#endif
