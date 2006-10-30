#ifndef XCORE_GAMEOVEREVENT_H
#define XCORE_GAMEOVEREVENT_H

#include "includes.h"
#include "Event.h"
#include "IEventListener.h"

namespace xcore
{

class IEventListener;

class GameOverEvent : public Event
{
   friend class boost::serialization::access;
   public:
      GameOverEvent(uint32 gameID = 0, uint32 winner = 0);
      virtual ~GameOverEvent();

      uint32 getGameID() const;
      uint32 getWinnerID() const;

      virtual void process(IEventListener* handler);

      template<class T>
      void serialize(T & archive, const unsigned int version);

   private:
      uint32 _winner;
      uint32 _gameID;
};

template<class T>
void GameOverEvent::serialize(T & archive, const unsigned int version)
{
   archive & boost::serialization::base_object<Event>(*this);
   archive & _winner;
   archive & _gameID;
}

} // namespace

#endif
