#ifndef XCORE_UNITCREATEEVENT_H
#define XCORE_UNITCREATEEVENT_H

#include "includes.h"
#include "Event.h"
#include "IEventListener.h"
#include "Item.h"

namespace xcore
{

class IEventListener;

class UnitCreateEvent : public Event
{
   friend class boost::serialization::access;
   public:
      UnitCreateEvent(uint32 playerID=-1, uint32 x=0, uint32 y=0, 
	itemtype s0 = (itemtype)0,
	itemtype s1 = (itemtype)0,
	itemtype s2 = (itemtype)0,
	itemtype s3 = (itemtype)0,
	itemtype s4 = (itemtype)0,
	itemtype s5 = (itemtype)0,
	itemtype s6 = (itemtype)0
	);
      virtual ~UnitCreateEvent();

      uint32 getPlayerID() const;
      uint32 getX() const;
      uint32 getY() const;
      itemtype getS0() const;
      itemtype getS1() const;
      itemtype getS2() const;
      itemtype getS3() const;
      itemtype getS4() const;
      itemtype getS5() const;
      itemtype getS6() const;

      virtual void process(IEventListener* handler);

      template<class T>
      void serialize(T & archive, const unsigned int version);

   private:
      uint32 _x;
      uint32 _y;
      uint32 _playerID;
      uint32 _temp;
      itemtype _s0;
      itemtype _s1;
      itemtype _s2;
      itemtype _s3;
      itemtype _s4;
      itemtype _s5;
      itemtype _s6;
      itemtype _s7;
};

template<class T>
void UnitCreateEvent::serialize(T & archive, const unsigned int version)
{
   archive & boost::serialization::base_object<Event>(*this);
   archive & _playerID;
   archive & _x;
   archive & _y;
   archive & _temp;
   archive & _s0;
   archive & _s1;
   archive & _s2;
   archive & _s3;
   archive & _s4;
   archive & _s5;
   archive & _s6;
}

} // namespace

#endif
