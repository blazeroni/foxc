#ifndef XCORE_GAMEJOINEVENT_H
#define XCORE_GAMEJOINEVENT_H

#include "includes.h"
#include "Event.h"
#include "IEventListener.h"

namespace xcore
{

class IEventListener;

class GameJoinEvent : public Event
{
   friend class boost::serialization::access;
   public:
      GameJoinEvent(string gameName = "", uint32 points = 0, uint16 playerNum = 0);
      virtual ~GameJoinEvent();

      void setJoined(bool val);

      string getGameName() const;
      uint32 getMaxPoints() const;

      uint16 getPlayerNumber() const;
      void setPlayerNumber(uint16 num);

      void setMaxPoints(uint32 points);
      bool hasJoined() const;

      virtual void process(IEventListener* handler);

      template<class T>
      void serialize(T & archive, const unsigned int version);

   private:
      string _gameName;
      bool _joined;
      uint32 _points;
      uint16 _playerNum;
};

template<class T>
void GameJoinEvent::serialize(T & archive, const unsigned int version)
{
   archive & boost::serialization::base_object<Event>(*this);
   archive & _joined;
   archive & _points;
   archive & _gameName;
   archive & _playerNum;
}

} // namespace

#endif
