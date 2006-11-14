#ifndef XCORE_STARTGAMEEVENT_H
#define XCORE_STARTGAMEEVENT_H

#include "includes.h"
#include "IEventListener.h"
#include "Event.h"

namespace xcore
{

class IEventListener;

class StartGameEvent : public Event
{
   friend class boost::serialization::access;
   public:
      StartGameEvent(uint32 seed = 0);
      virtual ~StartGameEvent();

      bool _dummy;

      virtual void process(IEventListener* handler);

      template<class T>
      void serialize(T & archive, const unsigned int version);

      uint32 getSeed() const;

    private:
	uint32 _seed;
};

template<class T>
void StartGameEvent::serialize(T & archive, const unsigned int version) {
   archive & boost::serialization::base_object<Event>(*this);
   archive & _dummy;
   archive & _seed;
}

} // namespace

#endif
