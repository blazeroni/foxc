#ifndef XCORE_USEMAPOBJECTEVENT_H
#define XCORE_USEMAPOBJECTEVENT_H

#include "includes.h"
#include "IEventListener.h"
#include "Event.h"

namespace xcore
{

class IEventListener;

class UseMapObjectEvent : public Event
{
   friend class boost::serialization::access;
   public:
      UseMapObjectEvent(uint32 unitID = 0);
      virtual ~UseMapObjectEvent();

      uint32 getUnitID() const;

      virtual void process(IEventListener* handler);

      template<class T>
      void serialize(T & archive, const unsigned int version);

   private:
      uint32 _unitID;
};

template<class T>
void UseMapObjectEvent::serialize(T & archive, const unsigned int version) {
   archive & boost::serialization::base_object<Event>(*this);
   archive & _unitID;
}

} // namespace

#endif
