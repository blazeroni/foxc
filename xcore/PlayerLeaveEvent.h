#ifndef XCORE_PLAYERLEAVEEVENT_H
#define XCORE_PLAYERLEAVEEVENT_H

#include "includes.h"
#include "Event.h"
#include "IEventListener.h"

namespace xcore
{

class IEventListener;

class PlayerLeaveEvent : public Event
{
   friend class boost::serialization::access;
   public:
      PlayerLeaveEvent(string playerName = "", uint32 playerID = 0);
      virtual ~PlayerLeaveEvent();

      string getPlayerName() const;
      uint32 getPlayerID() const;

      virtual void process(IEventListener* handler);

      template<class T>
      void serialize(T & archive, const unsigned int version);

   private:
      string _playerName;
      uint32 _playerID;
};

template<class T>
void PlayerLeaveEvent::serialize(T & archive, const unsigned int version)
{
   archive & boost::serialization::base_object<Event>(*this);
   archive & _playerName;
   archive & _playerID;
}

} // namespace

#endif
