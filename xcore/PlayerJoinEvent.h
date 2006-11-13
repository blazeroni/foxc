#ifndef XCORE_PLAYERJOINEVENT_H
#define XCORE_PLAYERJOINEVENT_H

#include "includes.h"
#include "Event.h"
#include "IEventListener.h"

namespace xcore
{

class IEventListener;

class PlayerJoinEvent : public Event
{
   friend class boost::serialization::access;
   public:
      PlayerJoinEvent(string playerName = "", uint32 playerID = 0, uint16 playerNum = 0);
      virtual ~PlayerJoinEvent();

      string getPlayerName() const;
      uint32 getPlayerID() const;
      uint16 getPlayerNumber() const;

      virtual void process(IEventListener* handler);

      template<class T>
      void serialize(T & archive, const unsigned int version);

   private:
      string _playerName;
      uint32 _playerID;
      uint16 _playerNum;
};

template<class T>
void PlayerJoinEvent::serialize(T & archive, const unsigned int version)
{
   archive & boost::serialization::base_object<Event>(*this);
   archive & _playerName;
   archive & _playerID;
   archive & _playerNum;
}

} // namespace

#endif
