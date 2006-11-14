#ifndef XCLIENT_GAMEHOST_H
#define XCLIENT_GAMEHOST_H

#include "includes.h"
#include "Event.h"

namespace xcore
{

class GameHostEvent : public Event
{
   friend class boost::serialization::access;

   public:
      GameHostEvent(string gameName = "", string mapName = "");
      virtual ~GameHostEvent();
   
      string getGameName();
      string getMapName();

      virtual void process(IEventListener* handler);

      template<class T>
      void serialize(T & archive, const unsigned int version);

   private:
      //uint32 _gameID;
      string _gameName;
      string _mapName;
};

template<class T>
void GameHostEvent::serialize(T & archive, const unsigned int version)
{
   archive & boost::serialization::base_object<Event>(*this);
   //archive & _gameID;
   archive & _gameName;
   archive & _mapName;
}

} // namespace

#endif
