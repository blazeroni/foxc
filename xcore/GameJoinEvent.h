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
      GameJoinEvent(string gameName = "", bool host = false);
      virtual ~GameJoinEvent();

      void setJoined(bool val);

      string getGameName() const;
      bool isHost() const;
      bool hasJoined() const;

      virtual void process(IEventListener* handler);

      template<class T>
      void serialize(T & archive, const unsigned int version);

   private:
      string _gameName;
      bool _host;
      bool _joined;
};

template<class T>
void GameJoinEvent::serialize(T & archive, const unsigned int version)
{
   archive & boost::serialization::base_object<Event>(*this);
   archive & _gameName;
   archive & _host;
}

} // namespace

#endif
