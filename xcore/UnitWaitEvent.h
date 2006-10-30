#ifndef XCORE_UNITWAITEVENT_H
#define XCORE_UNITWAITEVENT_H

#include "includes.h"
#include "IEventListener.h"
#include "Event.h"

namespace xcore
{

class IEventListener;

class UnitWaitEvent : public Event
{
   friend class boost::serialization::access;
   public:
      UnitWaitEvent(uint32 unitID = 0);
      virtual ~UnitWaitEvent();

      uint32 getUnitID() const;

      virtual void process(IEventListener* handler);

      template<class T>
      void serialize(T & archive, const unsigned int version);

   private:
      uint32 _unitID;
};

template<class T>
void UnitWaitEvent::serialize(T & archive, const unsigned int version) {
   archive & boost::serialization::base_object<Event>(*this);
   archive & _unitID;
}

} // namespace

#endif
